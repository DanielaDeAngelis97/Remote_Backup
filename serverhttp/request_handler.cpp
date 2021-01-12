//
// Created by daniela on 02/11/20.
//
#include "request_handler.h"
#include <fstream>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "reply.h"
#include "request.h"
#include <filesystem>
#include "crypto.h"
#include <boost/algorithm/string.hpp>
#include <utility>
#include "mime_types.h"

namespace http::server3 {

    request_handler::request_handler(std::string doc_root)
            : doc_root_(std::move(doc_root)) {
    }


    void request_handler::handle_request(const request &req, reply &rep) {
        std::string emailpasswd = req.headers[2].value;
        std::string logindb = "../Login/LoginDB.csv";
        std::ifstream is(logindb.c_str(), std::ios::in | std::ios::binary);
        if (!is) {
            rep = reply::stock_reply(reply::login_db_error);
            return;
        }
        std::string line;
        std::string email;
        std::string passwd;
        int find = 0;
        while (std::getline(is, line, '\n') && (find == 0)) { //read data from file object and put it into string.
            std::istringstream iss(line);
            std::getline(iss, email, ',');
            passwd = line.substr(line.find(',') + 1);
            std::string auth = CalcSha512(email + passwd);
            std::string decbase64_auth = reinterpret_cast<char *>(decode64(emailpasswd));
            if (decbase64_auth == auth) {
                find = 1;
            }
        }
        if (find == 0) {
            rep = reply::stock_reply(reply::unauthorized);
            return;
        }

        /// Decode url to path.
        std::string request_path;
        if (!url_decode(req.uri, request_path)) {
            rep = reply::stock_reply(reply::bad_request);
            return;
        }

        /// Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/'
            || request_path.find("..") != std::string::npos) {
            rep = reply::stock_reply(reply::bad_request);
            return;
        }

        /// Determine the file extension.
        std::size_t last_slash_pos = request_path.find_last_of('/');
        std::size_t last_dot_pos = request_path.find_last_of('.');
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = request_path.substr(last_dot_pos + 1);
        }

        ///Decode method
        if (req.method == "GET") {
            // Open the file to send back.
            std::string full_path = doc_root_ + "/" + email + "/" + request_path;
            std::ifstream is_get(full_path.c_str(), std::ios::in | std::ios::binary);
            if (!is_get) {
                rep = reply::stock_reply(reply::not_found);
                return;
            }
            /// Fill out the reply to be sent to the client.
            char buf[512];
            while (is_get.read(buf, sizeof(buf)).gcount() > 0)
                rep.content.append(buf, is_get.gcount());
            if (req.content.empty()){
                /// Voglio ripristinare i file lato Client
                rep.content.append("EOF%\n");
                rep.status = reply::ok;
                rep.headers.resize(2);
                rep.headers[0].name = "Content-Length";
                rep.headers[0].value = std::to_string(rep.content.size());
                rep.headers[1].name = "Content-Type";
                rep.headers[1].value = mime_types::extension_to_type(extension);
            }else {
                std::string hashcontent = CalcSha512(rep.content);
                if (hashcontent == req.content) {
                    rep.status = reply::accepted;
                } else {
                    rep.status = reply::not_sync;
                }
            }
        } else if (req.method == "POST") {
            const std::string full_path = doc_root_ + "/" + email + "/" + request_path;
            if (!extension.empty()) {
                std::ifstream is_post(full_path.c_str(), std::ios::in | std::ios::binary);
                if (is_post) {  /// Se esiste, lo rimuovo per aggiornarlo
                    std::filesystem::remove_all(std::filesystem::path(full_path));
                    rep.status = reply::accepted;
                }
                std::string dir_tree = full_path.substr(0, full_path.find_last_of('/'));
                std::filesystem::create_directories(dir_tree);
                ///CREO FILE
                std::ofstream wf(full_path, std::ios::out | std::ofstream::binary);
                wf.write(req.content.c_str(), req.content.length());
                wf.close();
                if (!wf.good()) {
                    std::cout << "Error occurred at writing time!" << std::endl;
                    // Fill out the reply to be sent to the client.
                    rep.status = reply::internal_server_error;
                } else {
                    // Fill out the reply to be sent to the client.
                    rep.status = reply::created;
                }
            } else {
                if (req.headers[5].value.empty() &&
                    req.headers[6].value.empty()) { //controllo se invece voglio fare semplicemente autenticazione
                    ///Oltre ad autenticarmi, inserisco nella risposta anche i path presenti sul server, così in caso
                    ///di perdita totale dei dati su client sono capace di ripristinarli.
                    const std::string full_path = doc_root_ + "/" + email;
                    std::ifstream is_login(full_path.c_str(), std::ios::in | std::ios::binary);
                    if (is_login) {  /// Se esiste, lo rimuovo per aggiornarlo
                        std::string path_server;
                        for (auto &file : std::filesystem::recursive_directory_iterator(full_path)) {
                            path_server += file.path().string() + "EOF%\n";
                        }
                        rep.content.append(path_server);
                        rep.status = reply::ok;
                        rep.headers.resize(2);
                        rep.headers[0].name = "Content-Length";
                        rep.headers[0].value = std::to_string(rep.content.size());
                        rep.headers[1].name = "Content-Type";
                        rep.headers[1].value = mime_types::extension_to_type(extension);
                    } else {
                        rep.content.append("");
                        rep.status = reply::ok;
                        rep.headers.resize(2);
                        rep.headers[0].name = "Content-Length";
                        rep.headers[0].value = std::to_string(rep.content.size());
                        rep.headers[1].name = "Content-Type";
                        rep.headers[1].value = mime_types::extension_to_type("");
                    }
                } else {
                    std::filesystem::create_directories(full_path);
                    rep.status = reply::created;
                }
            }
        } else if (req.method == "DELETE") {
            if (request_path == "/synchronization") {
                std::vector<std::string> paths_client;
                std::string path_to_control_server;
                std::vector<std::string> paths_to_delete;
                boost::split(paths_client, req.content, boost::is_any_of(";"));
                std::string path = doc_root_ + "/" + email + paths_client[0]; //rappresenta il path della cartella che abbiamo deciso di osservare lato client
                std::ifstream is_delete(path.c_str(), std::ios::in | std::ios::binary);
                if (is_delete) {
                    for (auto &file : std::filesystem::recursive_directory_iterator(
                            path)) { //qui prendo i path contenuti
                        /* Controllo se il singolo path del server è contenuto della lista del client */
                        size_t pos = file.path().string().find(email);
                        path_to_control_server = file.path().string().substr(pos + email.size());
                        if (std::find(std::begin(paths_client), std::end(paths_client), path_to_control_server) !=
                            std::end(paths_client)) {
                            // myinput is included in mylist.
                        } else {
                            paths_to_delete.push_back(file.path().string());
                        }
                    }
                    if (!paths_to_delete.empty()) {
                        for (auto &i : paths_to_delete) {
                            std::filesystem::remove_all(std::filesystem::path(i));
                        }
                        rep.status = reply::accepted;
                    } else {
                        rep.status = reply::nothing_to_delete;
                    }
                } else {
                    rep.status = reply::nothing_to_delete;
                }
            } else {
                std::string path = doc_root_ + "/" + email + "/" + request_path;
                std::ifstream is2(path.c_str(), std::ios::in | std::ios::binary);
                if (is2) {
                    std::filesystem::remove_all(std::filesystem::path(path));
                    rep.status = reply::accepted;
                } else {
                    rep.status = reply::accepted;
                }
            }
        }
    }

    bool request_handler::url_decode(const std::string &in, std::string &out) {
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 3 <= in.size()) {
                    int value;
                    std::istringstream is(in.substr(i + 1, 2));
                    if (is >> std::hex >> value) {
                        out += static_cast<char>(value);
                        i += 2;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            } else if (in[i] == '+') {
                out += ' ';
            } else {
                out += in[i];
            }
        }
        return true;
    }

} // namespace http


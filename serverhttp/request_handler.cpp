//
// Created by daniela on 02/11/20.
//
#include "request_handler.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "mime_types.h"
#include "reply.h"
#include "request.h"
#include <filesystem>
#include "crypto.h"

namespace http {
    namespace server3 {


        request_handler::request_handler(const std::string& doc_root)
                : doc_root_(doc_root)
        {
        }


        void request_handler::handle_request(const request& req, reply& rep)
        {
            std::string emailpasswd = req.headers[2].value;
            std::string logindb = "/home/daniela/CLionProjects/serverhttp/Login/LoginDB.csv";
            std::ifstream is(logindb.c_str(), std::ios::in | std::ios::binary);
            std::string line;
            std::string email;
            std::string passwd;
            int find=0;
            while(std::getline(is, line, '\n') && (find==0)){ //read data from file object and put it into string.
                std::istringstream iss(line);
                std::getline(iss, email, ',');
                passwd = line.substr(line.find(",")+1);
                std::string auth= CalcSha512(email+passwd);
                std::string decbase64_auth = reinterpret_cast<char *>(decode64(emailpasswd));
                if(decbase64_auth == auth){
                    find=1;
                }
            }
            if(find==0){
                rep = reply::stock_reply(reply::unauthorized);
                return;
            }

        // Decode url to path.
            std::string request_path;
            if (!url_decode(req.uri, request_path))
            {
                rep = reply::stock_reply(reply::bad_request);
                return;
            }

            // Request path must be absolute and not contain "..".
            if (request_path.empty() || request_path[0] != '/'
                || request_path.find("..") != std::string::npos)
            {
                rep = reply::stock_reply(reply::bad_request);
                return;
            }

           /* // If path ends in slash (i.e. is a directory) then add "index.html".
            if (request_path[request_path.size() - 1] == '/')
            {
                request_path += "index.html";
            } */

            // Determine the file extension.
            std::size_t last_slash_pos = request_path.find_last_of("/");
            std::size_t last_dot_pos = request_path.find_last_of(".");
            std::string extension;
            if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
            {
                extension = request_path.substr(last_dot_pos + 1);
            }

            //Decode method
            if(req.method=="GET") {
                // Open the file to send back.
                std::string full_path = doc_root_ + "/" + email + "/" + request_path;
                std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
                if (!is)
                {
                    rep = reply::stock_reply(reply::not_found);
                    return;
                }
                // Fill out the reply to be sent to the client.
                char buf[512];
                while (is.read(buf, sizeof(buf)).gcount() > 0)
                    rep.content.append(buf, is.gcount());
                std::string hashcontent= CalcSha512(rep.content);
                if(hashcontent == req.content){
                    rep.status = reply::accepted;
                }
                else{
                    rep.status = reply::not_sync;
                }
            }
            else if(req.method=="POST") {
                const std::string full_path = doc_root_ + "/" + email + "/" + request_path;
                if (extension != ""){
                    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
                    if (is) {
                        std::filesystem::remove_all(std::filesystem::path(full_path));
                        rep.status = reply::accepted;
                    }
                    std::string dir_tree = full_path.substr(0, full_path.find_last_of("\\/"));
                    std::filesystem::create_directories(dir_tree);
                    //CREO FILE
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
                }
                else{
                    if(req.headers[5].value == "" && req.headers[6].value == "") { //controllo se invece voglio fare semplicemente autenticazione
                        rep.status = reply::accepted;
                    }
                    else{
                        std::filesystem::create_directories(full_path);
                        rep.status = reply::created;
                    }
                }
            }
            else if(req.method=="DELETE") {
                std::string path = doc_root_ + "/" + email + "/" + request_path;
                std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
                if (is) {
                    std::filesystem::remove_all(std::filesystem::path(path));
                    rep.status = reply::accepted;
                } else {
                    rep.status = reply::not_found;
                }
            }
        }

        bool request_handler::url_decode(const std::string& in, std::string& out)
        {
            out.clear();
            out.reserve(in.size());
            for (std::size_t i = 0; i < in.size(); ++i)
            {
                if (in[i] == '%')
                {
                    if (i + 3 <= in.size())
                    {
                        int value;
                        std::istringstream is(in.substr(i + 1, 2));
                        if (is >> std::hex >> value)
                        {
                            out += static_cast<char>(value);
                            i += 2;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (in[i] == '+')
                {
                    out += ' ';
                }
                else
                {
                    out += in[i];
                }
            }
            return true;
        }

    } // namespace server3
} // namespace http


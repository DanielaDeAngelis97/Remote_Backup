//
// Created by daniela on 04/01/21.
//

#ifndef ASYNCLIENTHTTP_CLIENT_H
#define ASYNCLIENTHTTP_CLIENT_H

#include <iostream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "FileWatcher.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "mime_types.h"
#include "crypto.h"
#include <list>
#include <csignal>
#include <cstdlib>

using namespace boost::filesystem;
using boost::asio::ip::tcp;

static int check_connection = 1;

class client {
public:
    int autherror;
    unsigned int statuscode;

    ///Costruttore classe Client
    client(boost::asio::io_context &io_context,
           const std::string &server, const std::string &path, const std::string &emailpasswd, const std::string &email,
           const std::string &typereq, const std::string &length, const std::string &typecont,
           const std::string &content);

private:
    void handle_resolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

    void handle_connect(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

    void handle_write_request(const boost::system::error_code &err);

    void handle_read_status_line(const boost::system::error_code &err);

    void handle_read_headers(const boost::system::error_code &err);

    void handle_read_content(const boost::system::error_code &err);

    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

};

///Funzione che permette di sincronizzare il client e il server
void checksync(const std::string &path, const std::string &auth, const std::string &email);

/// Metodo POST HTTP, usato per creare o modificare i file sul Server
void
post_method(const std::string &path, const std::string &auth, const std::string &email, const std::string &extension);

/// Metodo DELETE HTTP, usato per eliminare i file sul Server
void
delete_method(const std::string &path, const std::string &auth, const std::string &email, const std::string &extension);

/// Funzione utilizzata per effettuare la riconnessione al server ogni 30 secondi.
void reconnection(const std::string &path, const std::string &auth, const std::string &email);

#endif //ASYNCLIENTHTTP_CLIENT_H
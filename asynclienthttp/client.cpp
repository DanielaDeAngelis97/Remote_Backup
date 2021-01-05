//
// Created by daniela on 04/01/21.
//

#include "client.h"
int check_connection=1;
client::client(boost::asio::io_context &io_context,
               const std::string &server, const std::string &path, const std::string &emailpasswd,
               const std::string &email,
               const std::string &typereq, const std::string &length, const std::string &typecont,
               const std::string &content)
        : resolver_(io_context),
          socket_(io_context),
          statuscode(0),
          autherror(0) {

    /// Form the request. We specify the "Connection: close" header so that the
    /// server will close the socket after transmitting the response.
    /// This will allow us to treat all data up until the EOF as the content.

    std::ostream request_stream(&request_);
    request_stream << typereq << " " << path << "?HTTP/1.1\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Client: " << email << "\r\n";
    request_stream << "Auth: " << emailpasswd << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "Content-Length: " << length << "\r\n";
    request_stream << "Content-Type: " << typecont << "\r\n\r\n";
    request_stream << content << "EOF%\n";

    /// Start an asynchronous resolve to translate the server and service names
    /// into a list of endpoints.
    tcp::resolver::query query(server, "http");
    resolver_.async_resolve(query,
                            boost::bind(&client::handle_resolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::iterator));
}

void client::handle_resolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
        /// Attempt a connection to the first endpoint in the list. Each endpoint
        /// will be tried until we successfully establish a connection.
        socket_.async_connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234),
                              boost::bind(&client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
        check_connection++; //La connessione va a buon fine
    } else {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void client::handle_connect(const boost::system::error_code &err,
                            tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
        /// The connection was successful. Send the request.
        boost::asio::async_write(socket_, request_,
                                 boost::bind(&client::handle_write_request, this,
                                             boost::asio::placeholders::error));
    } else if (endpoint_iterator != tcp::resolver::iterator()) {
        /// The connection failed. Try the next endpoint in the list.
        socket_.close();
        socket_.async_connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234),
                              boost::bind(&client::handle_connect, this,
                                          boost::asio::placeholders::error, ++endpoint_iterator));
        check_connection++; //La connessione va a buon fine
    } else {
        std::cout << "Error: " << err.message() << "\n";
        /// La connessione non va a buon fine, quindi setto questa variabile a 0,
        /// in modo tale da poter ritentare successivamente la connessione verso il server, nel caso in cui
        /// si tratti di Transient error.
        check_connection = 0;
    }
}

void client::handle_write_request(const boost::system::error_code &err) {
    if (!err) {
        /// Read the response status line.
        boost::asio::async_read_until(socket_, response_, "\r\n",
                                      boost::bind(&client::handle_read_status_line, this,
                                                  boost::asio::placeholders::error));
    } else {
        std::cout << "Error: " << err.message() << "\n";
        if (err.message() == "Connection reset by peer" || err.message() == "system:104")
            check_connection = 0; /// Connection reset by peer, quindi si ritenta la connessione.
    }
}

void client::handle_read_status_line(const boost::system::error_code &err) {
    if (!err) {
        /// Check that response is OK.
        std::istream response_stream(&response_);
        std::string http_version;
        response_stream >> http_version;
        response_stream >> statuscode;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Invalid response\n";
            return;
        }
        if (statuscode != 200 && statuscode != 401 && statuscode != 504 && statuscode != 205) {
            std::cout << "Response returned with status code ";
            std::cout << statuscode << "\n";
            return;
        }
        if (statuscode == 401) {
            std::cout << "Authentication failed " << "\n";
            autherror = 1;
            return;
        }
        if (statuscode == 504) {
            std::cout << "Internal Server Error during the authentication. " << "\n";
            return;
        }
        if (statuscode == 205) {
            std::cout << "Nothing to delete from server side." << "\n";
            return;
        }
        /// Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                      boost::bind(&client::handle_read_headers, this,
                                                  boost::asio::placeholders::error));
    } else {
        std::cout << "Error: " << err << "\n";
    }
}

void client::handle_read_headers(const boost::system::error_code &err) {
    if (!err) {
        /// Process the response headers.
        std::istream response_stream(&response_);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        /// Write whatever content we already have to output.
        if (response_.size() > 0)
            std::cout << &response_;

        /// Start reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&client::handle_read_content, this,
                                            boost::asio::placeholders::error));
    } else {
        std::cout << "Error: " << err << "\n";
    }
}

void client::handle_read_content(const boost::system::error_code &err) {
    if (!err) {
        /// Write all of the data that has been read so far.
        std::cout << &response_;

        /// Continue reading remaining data until EOF.
        boost::asio::async_read(socket_, response_,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&client::handle_read_content, this,
                                            boost::asio::placeholders::error));
    } else if (err != boost::asio::error::eof) {
        std::cout << "Error: " << err << "\n";
    }
}

void checksync(const std::string &path, const std::string &auth, const std::string &email) {
    std::cout << "\n" << "Synchronization started \n\n";

    //In files_client salviamo tutti i path presenti lato client.
    //Il primo path è il path generico che abbiamo deciso di osservare.
    std::string files_client = path;
    for (auto &file : std::filesystem::recursive_directory_iterator(path)) {
        std::string single_path = file.path().string();
        std::replace(single_path.begin(), single_path.end(), '\\', '/');
        files_client += ";" + single_path; //concatenazione di path ottenuta tramite i ";"
    }

    ///Eliminazione dei file addizionali presenti nel Server e NON nel Client
    std::cout << "\n" << "Deleting any additional file located in Server and not in Client..." << "\n";
    boost::asio::io_context io_context2;
    std::string m = "DELETE";
    auto length = boost::lexical_cast<std::string>(files_client.size());
    client c{io_context2, "127.0.0.1", "/synchronization", auth, email, m, length, "",
             files_client};
    io_context2.run();

    /// A specific command that allows the client to verify whether or not
    /// the server already has a copy of a given file or folder
    std::string method = "GET";
    for (auto &file : std::filesystem::recursive_directory_iterator(path)) {
        boost::asio::io_context io_context;

        std::string single_path = file.path().string();
        std::replace(single_path.begin(), single_path.end(), '\\', '/');
        std::cout << single_path << "\n";

        std::ifstream b(single_path.c_str(), std::ios::in | std::ios::binary);
        char buf[512];
        std::string content;
        while (b.read(buf, sizeof(buf)).gcount() > 0)
            content.append(buf, b.gcount());
        std::size_t last_slash_pos = single_path.find_last_of('/');
        std::size_t last_dot_pos = single_path.find_last_of('.');
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = single_path.substr(last_dot_pos + 1);
        }
        auto length_get = boost::lexical_cast<std::string>(content.size());
        std::string typecont = mime_types::extension_to_type(extension);
        std::string hashcontent = CalcSha512(content);
        client c_get{io_context, "127.0.0.1", single_path, auth, email, method, length_get, typecont,
                     hashcontent};
        io_context.run();

        ///Controllo il codice di ritorno. Se corrisponde a 404 (not found) o 305 (not_sync)
        /// il client esegue una post per aggiornare l'immagine dei file monitorati nel filesystem del Server.
        if (c_get.statuscode == 404 || c_get.statuscode == 305) {
            std::cout << "Updating server database..." << "\n";
            post_method(single_path, auth, email, extension);
        }
    }

    std::cout << "\n" << "Synchronization terminated" << "\n";
}

/// Metodo POST HTTP, usato per creare o modificare i file sul Server
void post_method(const std::string &path, const std::string &auth, const std::string &email,
                 const std::string &extension) { //Creiamo il metodo per la post
    boost::asio::io_context io_context;
    std::string method = "POST";
    std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
    // Fill out the request to be sent to the server.
    char buf[512];
    std::string content;
    while (is.read(buf, sizeof(buf)).gcount() > 0)
        content.append(buf, is.gcount());
    auto length = boost::lexical_cast<std::string>(content.size());
    std::string typecont = mime_types::extension_to_type(extension);
    client c{io_context, "127.0.0.1", path, auth, email, method, length, typecont,
             content};
    io_context.run();
}

/// Metodo DELETE HTTP, usato per eliminare i file sul Server
void delete_method(const std::string &path, const std::string &auth, const std::string &email,
                   const std::string &extension) { //Creiamo il metodo per la delete
    boost::asio::io_context io_context;
    std::string method = "DELETE";
    client c{io_context, "127.0.0.1", path, auth, email, method, "", "", " "};
    io_context.run();
}

void reconnection(const std::string &path, const std::string &auth, const std::string &email) {
    int i = 0; ///flag che indica il tempo trascorso da quanto sto provando a fare la reconnection. Dopo 3 minuti si chiude l'applicazione lato Client.
    boost::asio::io_context io_context1;
    boost::asio::deadline_timer timer(io_context1, boost::posix_time::seconds(30));
    std::function<void(const boost::system::error_code &)> tick = [&timer, &tick, &i, path, auth, email](
            const boost::system::error_code &e) {
        if (check_connection == 0) {
            std::cout << "Try to reconnect..." << "\n";
            i++;
            boost::asio::io_context io_context;
            client c{io_context, "127.0.0.1", "/reconnect", auth, email, "POST", "", "", ""};
            io_context.run();
        }
        if (check_connection == 1) {//prima connessione che va a buon fine dopo l'errore
            i = 0; ///resettiamo il timer
            checksync(path, auth, email);
        }
        if (i == 6) {
            i = 0;
            std::cout << "Server unreachable. Try to reconnect later. " << "\n" << "Closing the application...";
            exit(1);
        }
        timer.expires_at(timer.expires_at() + boost::posix_time::seconds(30));
        timer.async_wait(tick);
    };
    timer.async_wait(tick);
    io_context1.run();
}

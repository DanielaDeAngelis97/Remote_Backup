
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

using namespace boost::filesystem;

using boost::asio::ip::tcp;

int check_connection=1;
std::list <std::string> deleted_files;

class client
{
public:
    client(boost::asio::io_context& io_context,
           const std::string& server, const std::string& path, const std::string& emailpasswd, const std::string& email, std::string typereq, std::string length, std::string typecont, std::string content)
            : resolver_(io_context),
              socket_(io_context)
    {
        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
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

        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        tcp::resolver::query query(server, "http");
        resolver_.async_resolve(query,
                                boost::bind(&client::handle_resolve, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::iterator));
    }
    int autherror=0;
    int statuscode;

private:
    void handle_resolve(const boost::system::error_code& err,
                        tcp::resolver::iterator endpoint_iterator)
    {
        if (!err)
        {
            // Attempt a connection to the first endpoint in the list. Each endpoint
            // will be tried until we successfully establish a connection.
            socket_.async_connect(tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ),
                                  boost::bind(&client::handle_connect, this,
                                              boost::asio::placeholders::error, ++endpoint_iterator));
            check_connection++; //La connessione va a buon fine
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_connect(const boost::system::error_code& err,
                        tcp::resolver::iterator endpoint_iterator)
    {
        if (!err)
        {
            // The connection was successful. Send the request.
            boost::asio::async_write(socket_, request_,
                                     boost::bind(&client::handle_write_request, this,
                                                 boost::asio::placeholders::error));
        }
        else if (endpoint_iterator != tcp::resolver::iterator())
        {
            // The connection failed. Try the next endpoint in the list.
            socket_.close();
            socket_.async_connect(tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ),
                                  boost::bind(&client::handle_connect, this,
                                              boost::asio::placeholders::error, ++endpoint_iterator));
            check_connection++; //La connessione va a buon fine
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
            check_connection=0; //la connessione non va a buon fine.
        }
    }
    void handle_write_request(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Read the response status line.
            boost::asio::async_read_until(socket_, response_, "\r\n",
                                          boost::bind(&client::handle_read_status_line, this,
                                                      boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_read_status_line(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Check that response is OK.
            std::istream response_stream(&response_);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);
            if (!response_stream || http_version.substr(0, 5) != "HTTP/")
            {
                std::cout << "Invalid response\n";
                return;
            }
            if (status_code != 200 && status_code != 401 && status_code != 504 )
            {
                std::cout << "Response returned with status code ";
                std::cout << status_code << "\n";
                statuscode = status_code;
                return;
            }
            if (status_code == 401){
                std::cout << "Authentication failed "<< "\n";
                autherror =1;
                return;
            }
            if (status_code == 504){
                statuscode = status_code;
                std::cout << "Internal Server Error during the authentication. "<< "\n";
                return;
            }
            // Read the response headers, which are terminated by a blank line.
            boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
                                          boost::bind(&client::handle_read_headers, this,
                                                      boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_headers(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Process the response headers.
            std::istream response_stream(&response_);
            std::string header;
            while (std::getline(response_stream, header) && header != "\r")
                std::cout << header << "\n";
            std::cout << "\n";

            // Write whatever content we already have to output.
            if (response_.size() > 0)
                std::cout << &response_;

            // Start reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_content(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Write all of the data that has been read so far.
            std::cout << &response_;

            // Continue reading remaining data until EOF.
            boost::asio::async_read(socket_, response_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        }
        else if (err != boost::asio::error::eof)
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

};
void checksync (std::string path, const std::string& auth, std::string email);
void post_method (std::string path, const std::string& auth, std::string email, std::string extension);
void delete_method (std::string path, const std::string& auth, std::string email, std::string extension);

void checksync (std::string path, const std::string& auth, std::string email) {
    std::cout<< "\n" << "Synchronization started \n\n" ;

    for (std::string x: deleted_files){
        std::size_t last_slash_pos = x.find_last_of("/");
        std::size_t last_dot_pos = x.find_last_of(".");
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = x.substr(last_dot_pos + 1);
        }
        std::cout << "\n" << "Deleting file "<< x << "\n";
        delete_method (x, auth,  email, extension);
    }
    deleted_files.clear();

    std::string method = "GET";
    std::string files_client = path; //il primo path è il path generico che abbiamo deciso di osservare
    for (auto &file : std::filesystem::recursive_directory_iterator(path)) {
        boost::asio::io_context io_context;

        std::string single_path = file.path().string();
        files_client= files_client + ";" + single_path ;
        std::replace(single_path.begin(), single_path.end(), '\\', '/');
        std::cout<< single_path << "\n";

        std::ifstream b(single_path.c_str(), std::ios::in | std::ios::binary);
        char buf[512];
        std::string content;
        while (b.read(buf, sizeof(buf)).gcount() > 0)
            content.append(buf, b.gcount());
        std::size_t last_slash_pos = single_path.find_last_of("/");
        std::size_t last_dot_pos = single_path.find_last_of(".");
        std::string extension;
        if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
            extension = single_path.substr(last_dot_pos + 1);
        }
        std::string length = boost::lexical_cast<std::string>(content.size());
        std::string typecont = mime_types::extension_to_type(extension);
        std::string hashcontent = CalcSha512(content);
        client c{io_context, "127.0.0.1", single_path, auth, email, method, length, typecont,
                 hashcontent};
        io_context.run();

        //Controllo il codice di errore:
        if(c.statuscode==404 || c.statuscode==305){
            std::cout<< "Updating server database..." << "\n";
            post_method (single_path, auth, email, extension);
        }
    }

    std::cout << "\n" << "Deleting any additional file located in Server and not in Client..." << "\n";
    boost::asio::io_context io_context2;
    std::string m= "DELETE";
    std::string content = files_client;
    std::string length = boost::lexical_cast<std::string>(content.size());
    client c{io_context2, "127.0.0.1", "/synchronization", auth, email, m, length, "",
             content};
    io_context2.run();
    std::cout << "\n" << "Synchronization terminated" << "\n";
}

void post_method (std::string path, const std::string& auth, std::string email, std::string extension) { //Creiamo il metodo per la post
    boost::asio::io_context io_context;
    std::string method = "POST";
    std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
    // Fill out the request to be sent to the server.
    char buf[512];
    std::string content;
    while (is.read(buf, sizeof(buf)).gcount() > 0)
        content.append(buf, is.gcount());
    std::string length = boost::lexical_cast<std::string>(content.size());
    std::string typecont = mime_types::extension_to_type(extension);
    client c{io_context, "127.0.0.1", path, auth, email, method, length, typecont,
             content};
    io_context.run();
}

void delete_method (std::string path, const std::string& auth, std::string email, std::string extension) { //Creiamo il metodo per la delete
    boost::asio::io_context io_context;
    std::string method = "DELETE";
    client c{io_context, "127.0.0.1", path, auth, email, method, "", "", " "};
    io_context.run();
}
void reconnection (const std::string& auth, std::string email) { ///Creiamo il metodo per effettuare la riconnessione al server
    boost::asio::io_context io_context1;
    boost::asio::deadline_timer timer(io_context1, boost::posix_time::seconds(30));
    std::function<void(const boost::system::error_code&)> tick = [&timer,&tick, auth, email](const boost::system::error_code& e){
        if(check_connection == 0){
            std::cout << "Try to reconnect..." << "\n";
            boost::asio::io_context io_context;
            client c{io_context, "127.0.0.1", "/reconnect", auth, email, "POST", "", "", ""};
            io_context.run();
        }
        timer.expires_at(timer.expires_at() + boost::posix_time::seconds(30));
        timer.async_wait(tick);
    };
    timer.async_wait(tick);
    io_context1.run();
}


void signalHandler( int signum) {
    std::cout << "\n" << "Closing the applications..."<<"\n";
    // cleanup and close up stuff here
    // terminate program
    exit(signum);
}

int main(int argc, char* argv[])
{
    std::string response= "y";
    int exit=0;
    std::string path;
    std::string email;
    std::string passwd;
    std::string auth;
    char *base64auth;
    int tentativi=0;
    while (response == "y" && exit==0) {
        std::cout << "Insert email: " << "\n";
        std::cin >> email;
        std::cout << "Insert password: " << "\n";
        std::cin >> passwd;
        boost::asio::io_context io_context;

        std::string passwdhash = CalcSha512(passwd);
        auth = CalcSha512(email + passwdhash);
        base64auth = base64(auth);
        client c{io_context, "127.0.0.1", "/login", base64auth, email, "POST", "", "", ""};
        io_context.run();
        if(c.statuscode == 504){  //Internal server error DB
            std::cout << "Try to connect later..." << "\n";
            response = 'n';
        }
        if(check_connection == 0){
            std::cout << "Server down. Try to connect later..." << "\n";
            response = 'n';
        } else {
            if (c.autherror == 0) {
                exit = 1;
            } else {
                tentativi++;
                if (tentativi < 3) {
                    std::cout << "Incorrect login credentials. Do you want to retry? y/n ";
                    std::cin >> response;
                } else {
                    std::cout << "You have exhausted the attempts available! ";
                    response = 'n';
                }
            }
        }
    }

    if ( response == "y") {
        std::cout << "Insert path to watch: " << "\n";
        std::getline(std::cin >> std::ws, path);
        while (!std::filesystem::is_directory(std::filesystem::path(path))){
            std::cout << "This path doesn't exist. Retry." << "\n";
            std::cout << "Insert path to watch: " << "\n";
            std::getline(std::cin >> std::ws, path);
        }

        // Create a FileWatcher instance that will check the current folder for changes every 5 seconds
        FileWatcher fw{path, std::chrono::milliseconds(1000)};

        std::future<void> f = std::async(std::launch::async, checksync, path, base64auth, email);

        bool running_ = true;
        int b=30; //variabile timer che ci permette di definire 1 minuto
        std::chrono::duration<int, std::milli> delay= std::chrono::milliseconds(1000);

        //Verifico se la connessione si è ripresa, facendo una connesione prova ogni 30 secondi
        std::future<void> f3 = std::async(std::launch::async, reconnection, base64auth, email);

        while(running_) {
            // Wait for "delay" milliseconds
            std::this_thread::sleep_for(delay) ;

            //per uscire dall'applicazione
            signal(SIGINT, signalHandler);
            if(check_connection == 1){ //prima connessione che va a buon fine dopo l'errore
                std::future<void> f2 = std::async(std::launch::async, checksync, path, base64auth, email);
            }
            // Start monitoring a folder for changes and (in case of changes)
            // run a user provided lambda function
            try {
                fw.start([base64auth, email, path](const std::string &path_to_watch, FileStatus status) -> void {
                    // Sostituzione Slash Windows
                    std::string single_path = path_to_watch;

                    std::replace(single_path.begin(), single_path.end(), '\\', '/');

                    if(check_connection == 1){ //prima connessione che va a buon fine dopo l'errore
                        std::future<void> f4 = std::async(std::launch::async, checksync, path, base64auth, email);
                    }

                    // Process only regular files, all other file types are ignored
                    if (!std::filesystem::is_regular_file(std::filesystem::path(single_path)) &&
                        !std::filesystem::is_directory(std::filesystem::path(single_path)) &&
                        status != FileStatus::erased) {
                        return;
                    }
                    // Determine the file extension.
                    std::size_t last_slash_pos = single_path.find_last_of("/");
                    std::size_t last_dot_pos = single_path.find_last_of(".");
                    std::string extension;
                    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
                        extension = single_path.substr(last_dot_pos + 1);
                    }
                    switch (status) {
                        case FileStatus::created: {
                            if (std::filesystem::is_regular_file(std::filesystem::path(single_path)))
                                std::cout << "File created: " << single_path << '\n';
                            else
                                std::cout << "Folder created: " << single_path << '\n';
                            try {
                                post_method (single_path, base64auth, email, extension);
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                            }
                        }
                            break;
                        case FileStatus::modified: {
                            if (extension != "")
                                std::cout << "File modified: " << single_path << '\n';
                            else
                                std::cout << "Folder modified: " << single_path << '\n';
                            try {
                                post_method (single_path, base64auth, email, extension);
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                            }
                        }
                            break;
                        case FileStatus::erased: {
                            if (extension != "")
                                std::cout << "File erased: " << single_path << '\n';
                            else
                                std::cout << "Folder erased: " << single_path << '\n';
                            try {
                                delete_method (single_path, base64auth, email, extension);
                                if(check_connection == 0) {
                                    deleted_files.push_back(single_path);
                                }
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                            }
                        }
                            break;
                        default:
                            std::cout << "Error! Unknown file status.\n";
                    }
                });
            }
            catch (std::exception &e) {
                std::cout << "Exception: " << e.what() << "\n";
            }
        }

    }


    return 0;
}

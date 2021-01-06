
#include <iostream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include "FileWatcher.h"
#include "crypto.h"
#include <list>
#include <csignal>
#include <cstdlib>
#include "client.h"

using namespace boost::filesystem;

using boost::asio::ip::tcp;


///Funzione che viene usata per terminare correttamente l'esecuzione del Client dopo la ricezione del segnale SIGINT.
void signalHandler(int signum) {
    std::cout << "\n" << "Closing the applications..." << "\n";
    // cleanup and close up stuff here
    // terminate program
    exit(signum);
}


int main() {
    std::string response = "y";
    int exit_from_while = 0;
    std::string path;
    std::string email;
    std::string passwd;
    std::string auth;
    std::string paths_server;
    char *base64auth;
    int tentativi = 0;
    while (response == "y" && exit_from_while == 0) {   ///Controlliamo che le credenziali inserite siano corrette
        std::cout << "Insert email: " << std::endl;
        std::cin >> email;
        std::cout << "Insert password: " << std::endl;
        std::cin >> passwd;
        boost::asio::io_context io_context;

        /* Come previsto dal protocollo applicativo http, per effettuare un'operazione di autenticazione è necessario inviare
         * tra gli header della richiesta la stringa: email+passwd, codificato in base64.
         * Nello specifico, per garantire una maggiore sicurezza, sia nel DB, sia nella trasmissione dei dati, si è deciso di :
         * 1) Inserire nel DB una passwd cifrata tramite l'algoritmo sha512;
         * 2) Calcolare l'hash della stringa (email+passwd)
         * 3) Codificare in base64 la stringa precedentemente ottenuta, in modo da rimanere conformi agli standard.
        */
        std::string passwdhash = CalcSha512(passwd);
        auth = CalcSha512(email + passwdhash);
        base64auth = base64(auth);
        client c{io_context, "127.0.0.1", "/login", base64auth, email, "POST", "", "", ""};
        io_context.run();
        paths_server = c.response_content;

        if (c.statuscode == 504) {  ///Internal server error DB
            std::cout << "Try to connect later..." << "\n";
            response = 'n';
        }

        /* Controlliamo la connessione con il server. Se non va a buon fine vuol dire che già al primo tentativo di
         * connessione ad esso, non risulta raggiungibile e si chiude l'applicazione chiedendo di riprovare successivamente.
         */
        if (check_connection == 0) {
            std::cout << "Server down. Try to connect later..." << "\n";
            response = 'n';
        } else {
            if (c.autherror == 0) {   /*Autenticazione corretta -> si esce dal ciclo e si passa allo step successivo*/
                exit_from_while = 1;
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

    if (response == "y") {
        std::cout << "Insert path to watch: " << "\n";
        std::getline(std::cin >> std::ws, path);
        int restore=0;
        std::future<void> f;
        while (!std::filesystem::is_directory(std::filesystem::path(path))) {
            std::cout << "This path doesn't exist. Retry." << "\n";
            std::cout << "Insert path to watch: " << "\n";
            std::getline(std::cin >> std::ws, path);
        }

        if ( (std::filesystem::is_empty(std::filesystem::path(path))) &&  (paths_server.length()!=0) ){

            restore_client (path, base64auth, email, paths_server);
            restore=1;
        }
        /// Create a FileWatcher instance that will check the current folder for changes every 5 seconds
        FileWatcher fw{path};

        /// Se il client ha perso tutti i dati chiamo la funzione -> restore_client
        /// altrimenti -> checksync
        if ( restore==0 ) {
            /// In modo asincrono richiamiamo la funzione checksync
           f = std::async(std::launch::async, checksync, path, base64auth, email);
        }

        bool running_ = true;
        std::chrono::duration<int, std::milli> delay = std::chrono::milliseconds(1000);

        ///Verifico se la connessione si è ripresa, facendo una connesione prova ogni 30 secondi
        std::future<void> f3 = std::async(std::launch::async, reconnection, path, base64auth, email);

        while (running_) {
            /// Wait for "delay" milliseconds
            std::this_thread::sleep_for(delay);

            /// Per uscire dall'applicazione
            signal(SIGINT, signalHandler);

            /// Start monitoring a folder for changes and (in case of changes)
            /// run a user provided lambda function
            try {
                fw.start([base64auth, email, path](const std::string &path_to_watch, FileStatus status) -> void {
                    // Sostituzione Slash Windows
                    std::string single_path = path_to_watch;

                    std::replace(single_path.begin(), single_path.end(), '\\', '/');
                    /// Process only regular files and directories, all other file types are ignored
                    if (!std::filesystem::is_regular_file(std::filesystem::path(single_path)) &&
                        !std::filesystem::is_directory(std::filesystem::path(single_path)) &&
                        status != FileStatus::erased) {
                        return;
                    }
                    /// Determine the file extension.
                    std::size_t last_slash_pos = single_path.find_last_of('/');
                    std::size_t last_dot_pos = single_path.find_last_of('.');
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
                                post_method(single_path, base64auth, email, extension);
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                                exit(EXIT_FAILURE);
                            }
                        }
                            break;
                        case FileStatus::modified: {
                            if (!extension.empty())
                                std::cout << "File modified: " << single_path << '\n';
                            else
                                std::cout << "Folder modified: " << single_path << '\n';
                            try {
                                post_method(single_path, base64auth, email, extension);
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                                exit(EXIT_FAILURE);
                            }
                        }
                            break;
                        case FileStatus::erased: {
                            if (!extension.empty())
                                std::cout << "File erased: " << single_path << '\n';
                            else
                                std::cout << "Folder erased: " << single_path << '\n';
                            try {
                                delete_method(single_path, base64auth, email, extension);
                            }
                            catch (std::exception &e) {
                                std::cout << "Exception: " << e.what() << "\n";
                                exit(EXIT_FAILURE);
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
                exit(EXIT_FAILURE);
            }
        }

    }

    return 0;
}

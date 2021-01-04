#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include "server.h"
#include <csignal>


#if !defined(_WIN32)

int main(int argc, char *argv[]) {
    try {
        // Check command line arguments.
        if (argc != 4) {
            std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 .\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 .\n";
            return 1;
        } else {
            std::cout << "Server ready" << std::endl;
        }

        // Block all signals for background thread.
        sigset_t new_mask;
        sigfillset(&new_mask);
        sigset_t old_mask;
        pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

        // Run server in background thread.
        std::size_t num_threads = std::thread::hardware_concurrency();
        http::server3::server s(argv[1], argv[2], argv[3], num_threads);
        boost::thread t(boost::bind(&http::server3::server::run, &s));

        // Restore previous signals.
        pthread_sigmask(SIG_SETMASK, &old_mask, nullptr);

        // Wait for signal indicating time to shut down.
        sigset_t wait_mask;
        sigemptyset(&wait_mask);
        sigaddset(&wait_mask, SIGINT);
        sigaddset(&wait_mask, SIGQUIT);
        sigaddset(&wait_mask, SIGTERM);
        pthread_sigmask(SIG_BLOCK, &wait_mask, nullptr);
        int sig = 0;
        sigwait(&wait_mask, &sig);

        // Stop the server.
        s.stop();
        t.join();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }

    return 0;
}


#elif defined(_WIN32)

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    console_ctrl_function();
    return TRUE;
  default:
    return FALSE;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 4)
    {
      std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    http_server 0.0.0.0 80 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    http_server 0::0 80 .\n";
      return 1;
    }

    // Initialise server.
    std::size_t num_threads = std::thread::hardware_concurrency();
    http::server3::server s(argv[1], argv[2], argv[3], num_threads);

    // Set console control handler to allow server to be stopped.
    console_ctrl_function = boost::bind(&http::server3::server::stop, &s);
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

#endif // defined(_WIN32)

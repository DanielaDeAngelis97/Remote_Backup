//
// Created by daniela on 02/11/20.
//

#include "server.h"
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace http {
    namespace server3 {

        server::server(const std::string& address, const std::string& port,
                       const std::string& doc_root, std::size_t thread_pool_size)
                : thread_pool_size_(thread_pool_size),
                  acceptor_(io_service_),
                  new_connection_(new connection(io_service_, request_handler_)),
                  request_handler_(doc_root)
        {
            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query(address, port);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();
            acceptor_.async_accept(new_connection_->socket(),
                                   boost::bind(&server::handle_accept, this,
                                               boost::asio::placeholders::error));
        }

        void server::run()
        {
            // Create a pool of threads to run all of the io_services.
            std::vector<boost::shared_ptr<boost::thread> > threads;
            for (std::size_t i = 0; i < thread_pool_size_; ++i)
            {
                boost::shared_ptr<boost::thread> thread(new boost::thread(
                        boost::bind(&boost::asio::io_service::run, &io_service_)));
                threads.push_back(thread);
            }

            // Wait for all threads in the pool to exit.
            for (std::size_t i = 0; i < threads.size(); ++i)
                threads[i]->join();
        }

        void server::stop()
        {
            io_service_.stop();
        }

        void server::handle_accept(const boost::system::error_code& e)
        {
            if (!e)
            {
                new_connection_->start();
                //Destroys the object currently managed by the unique_ptr (if any) and takes ownership of p
                new_connection_.reset(new connection(io_service_, request_handler_)); //
                acceptor_.async_accept(new_connection_->socket(),
                                       boost::bind(&server::handle_accept, this,
                                                   boost::asio::placeholders::error));
            }
        }

    } // namespace server3
} // namespace http
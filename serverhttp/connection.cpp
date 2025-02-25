//
// Created by daniela on 02/11/20.
//
#include "connection.h"
#include <vector>
#include <boost/bind/bind.hpp>
#include "request_handler.h"
#include <iostream>

namespace http {
    namespace server3 {

        connection::connection(boost::asio::io_context& io_context,
                               request_handler& handler)
                : strand_(io_context),
                  socket_(io_context),
                  request_handler_(handler)
        {
        }

        boost::asio::ip::tcp::socket& connection::socket()
        {
            return socket_;
        }

        void connection::start()
        {
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    strand_.wrap(
                                            boost::bind(&connection::handle_read, shared_from_this(),
                                                        boost::asio::placeholders::error,
                                                        boost::asio::placeholders::bytes_transferred)));
        }

        void connection::handle_read(const boost::system::error_code& e,
                                     std::size_t bytes_transferred)
        {
            if (!e && socket_.is_open())
            {
                boost::tribool result;
                boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
                        request_, buffer_.data(), buffer_.data() + bytes_transferred);
                std::cout<< request_.uri << " sto facendo queste cose " << "\n";

                if (result && socket_.is_open())
                {
                    request_handler_.handle_request(request_, reply_);
                    boost::asio::async_write(socket_, reply_.to_buffers(),
                                             strand_.wrap(
                                                     boost::bind(&connection::handle_write, shared_from_this(),
                                                                 boost::asio::placeholders::error)));
                }
                else if (!result || !(socket_.is_open()))
                {
                    reply_ = reply::stock_reply(reply::bad_request);
                    boost::asio::async_write(socket_, reply_.to_buffers(),
                                             strand_.wrap(
                                                     boost::bind(&connection::handle_write, shared_from_this(),
                                                                 boost::asio::placeholders::error)));
                }
                else
                {
                    socket_.async_read_some(boost::asio::buffer(buffer_),
                                            strand_.wrap(
                                                    boost::bind(&connection::handle_read, shared_from_this(),
                                                                boost::asio::placeholders::error,
                                                                boost::asio::placeholders::bytes_transferred)));
                }
            }

            // If an error occurs then no new asynchronous operations are started. This
            // means that all shared_ptr references to the connection object will
            // disappear and the object will be destroyed automatically after this
            // handler returns. The connection class's destructor closes the socket.
        }

        void connection::handle_write(const boost::system::error_code& e)
        {
            if (!e)
            {
                // Initiate graceful connection closure.
                boost::system::error_code ignored_ec;
                socket_.cancel();
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                std::cout << "SONO MORTO " << "\n";
            }

            // No new asynchronous operations are started. This means that all shared_ptr
            // references to the connection object will disappear and the object will be
            // destroyed automatically after this handler returns. The connection class's
            // destructor closes the socket.
        }

    } // namespace server3
} // namespace http


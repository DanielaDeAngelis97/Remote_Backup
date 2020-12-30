//
// Created by daniela on 02/11/20.
//
#include "connection.h"
#include <vector>
#include <boost/bind/bind.hpp>
#include "request_handler.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <iomanip>

namespace http {
    namespace server3 {
        float percentuale=0.00;
        float byte_intermedi=0.00;
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
            if (!e)
            {
                boost::tribool result;
                boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
                        request_, buffer_.data(), buffer_.data() + bytes_transferred);
                if (request_.uri != "/login" && request_.uri != "/synchronization" && request_.uri != "/reconnect" && request_.method == "POST"){
                    byte_intermedi = byte_intermedi + bytes_transferred;
                percentuale = (byte_intermedi/(std::stol(request_.headers[5].value, nullptr, 10)))*100;
                std::cout << request_.uri << ": " << std::fixed << std::setprecision(2) << percentuale << "%" <<"\n";
                }

                if (result)
                {
                    percentuale=0.00;
                    byte_intermedi=0.00;
                    request_handler_.handle_request(request_, reply_);
                    boost::asio::async_write(socket_, reply_.to_buffers(),
                                             strand_.wrap(
                                                     boost::bind(&connection::handle_write, shared_from_this(),
                                                                 boost::asio::placeholders::error)));
                }
                else if (!result)
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
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            }

            // No new asynchronous operations are started. This means that all shared_ptr
            // references to the connection object will disappear and the object will be
            // destroyed automatically after this handler returns. The connection class's
            // destructor closes the socket.
        }

    } // namespace server3
} // namespace http


//
// Created by daniela on 02/11/20.
//

#ifndef SERVERHTTP_REQUEST_HANDLER_H
#define SERVERHTTP_REQUEST_HANDLER_H


#include <string>
#include <boost/noncopyable.hpp>

namespace http {
    namespace server3 {

        struct reply;
        struct request;

/// The common handler for all incoming requests.
        class request_handler
                : private boost::noncopyable
        {
        public:
            /// Construct with a directory containing files to be served.
            explicit request_handler(const std::string& doc_root);

            /// Handle a request and produce a reply.
            void handle_request(const request& req, reply& rep);

        private:
            /// The directory containing the files to be served.
            std::string doc_root_;

            /// Perform URL-decoding on a string. Returns false if the encoding was
            /// invalid.
            static bool url_decode(const std::string& in, std::string& out);
        };

    } // namespace server3
} // namespace http

#endif //SERVERHTTP_REQUEST_HANDLER_H

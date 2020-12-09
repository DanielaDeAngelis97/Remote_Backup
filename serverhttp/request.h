//
// Created by daniela on 02/11/20.
//

#ifndef SERVERHTTP_REQUEST_H
#define SERVERHTTP_REQUEST_H
#include <string>
#include <vector>
#include "header.h"

namespace http {
    namespace server3 {

/// A request received from a client.
        struct request
        {
            std::string method;
            std::string uri;
            int http_version_major;
            int http_version_minor;
            std::vector<header> headers;
            /// The content to be sent in the request
            std::string content;
        };

    } // namespace server3
} // namespace http

#endif //SERVERHTTP_REQUEST_H

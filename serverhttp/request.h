//
// Created by daniela on 02/11/20.
//

#ifndef SERVERHTTP_REQUEST_H
#define SERVERHTTP_REQUEST_H

#include <string>
#include <vector>
#include "header.h"

namespace http::server3 {

/// A request received from a client.
    struct request {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<header> headers;
        /// The content to be sent in the request
        std::string content;
        float percentage = 0.00;
        float intermediate_bytes = 0.00;
    };

} // namespace http

#endif //SERVERHTTP_REQUEST_H

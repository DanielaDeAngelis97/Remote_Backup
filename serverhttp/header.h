//
// Created by daniela on 02/11/20.
//

#ifndef SERVERHTTP_HEADER_H
#define SERVERHTTP_HEADER_H
#include <string>

namespace http {
    namespace server3 {

        struct header
        {
            std::string name;
            std::string value;
        };

    } // namespace server3
} // namespace http
#endif //SERVERHTTP_HEADER_H

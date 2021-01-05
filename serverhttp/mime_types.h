//
// Created by daniela on 02/11/20.
//

#ifndef SERVERHTTP_MIME_TYPES_H
#define SERVERHTTP_MIME_TYPES_H


#include <string>

namespace http::server3::mime_types {

/// Convert a file extension into a MIME type.
    std::string extension_to_type(const std::string& extension);
    //std::string type_to_extension(const std::string& mime_type);

} // namespace http

#endif //SERVERHTTP_MIME_TYPES_H

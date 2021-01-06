//
// Created by daniela on 02/11/20.
//

#pragma once
#include <string>

namespace mime_types {

/// Convert a file extension into a MIME type.
    std::string extension_to_type(const std::string &extension);

    //std::string type_to_extension(const std::string &mime_type);

} // namespace mime_types

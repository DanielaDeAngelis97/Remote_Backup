//
// Created by daniela on 02/11/20.
//

#include "mime_types.h"

namespace mime_types {

    struct mapping {
        const char *extension;
        const char *mime_type;
    } mappings[] =
            {
                    {"gif",  "image/gif"},
                    {"pdf",  "application/pdf"},
                    {"html", "text/html"},
                    {"jpg",  "image/jpeg"},
                    {"csv",  "text/csv"},
                    {"mp4",  "video/mp4"},
                    {"txt",  "text/plain"},
                    {"png",  "image/png"},
                    {"doc",  "application/msword"},
                    {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
                    {nullptr,      nullptr} // Marks end of list.
            };

    std::string extension_to_type(const std::string &extension) {
        for (mapping *m = mappings; m->extension; ++m) {
            if (m->extension == extension) {
                return m->mime_type;
            }
        }

        return "text/plain";
    }

  /*  std::string type_to_extension(const std::string &mime_type) {
        for (mapping *m = mappings; m->mime_type; ++m) {
            if (m->mime_type == mime_type) {
                return m->extension;
            }
        }

        return "txt";
    }*/


} // namespace mime_types


//
// Created by daniela on 09/12/20.
//

#include "crypto.h"
#include <optional>
#include <iomanip>
#include <openssl/sha.h>
#include <iostream>


std::string CalcSha512(const std::string &input) {
    // Initialize openssl
    SHA512_CTX context;
    SHA512_Init(&context);

    // Read file and update calculated SHA
    SHA512_Update(&context, (unsigned char *) input.c_str(), input.length());

    // Get Final SHA
    unsigned char result[SHA512_DIGEST_LENGTH];
    SHA512_Final(result, &context);

    // Transform byte-array to string
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto &byte: result) {
        shastr << std::setw(2) << (int) byte;
    }
    return shastr.str();
}

unsigned char *decode64(const std::string &input) {
    const auto pl = 3 * input.length() / 4;
    auto output = reinterpret_cast<unsigned char *>(calloc(pl + 1, 1));
    const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>((const char *) input.c_str()),
                                    input.length());
    if (pl != ol) {
        std::cerr << "Whoops, decode predicted " << pl << " but we got " << ol << "\n";
        exit(EXIT_FAILURE);
    }
    return output;
}


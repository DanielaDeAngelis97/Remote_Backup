//
// Created by daniela on 09/12/20.
//

#include "crypto.h"
#include <cstring>
#include <fstream>
#include <optional>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <iostream>


std::string CalcSha512(std::string input)
{
    // Initialize openssl
    SHA512_CTX context;
    SHA512_Init(&context);

    // Read file and update calculated SHA
    SHA512_Update(&context,(unsigned char*)input.c_str(), input.length());

    // Get Final SHA
    unsigned char result[SHA512_DIGEST_LENGTH];
    SHA512_Final(result, &context);

    // Transform byte-array to string
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto &byte: result)
    {
        shastr << std::setw(2) << (int)byte;
    }
    return shastr.str();
}

char *base64(std::string input) {
    const auto pl = 4*((input.length()+2)/3);
    auto output = reinterpret_cast<char *>(calloc(pl+1, 1)); //+1 for the terminating null that EVP_EncodeBlock adds on
    const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output), (unsigned char*)input.c_str(), input.length());
    if (pl != ol) { std::cerr << "Whoops, encode predicted " << pl << " but we got " << ol << "\n"; }
    return output;
}



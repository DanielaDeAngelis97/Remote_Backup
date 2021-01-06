//
// Created by daniela on 09/12/20.
//

#ifndef ASYNCLIENTHTTP_CRYPTO_H
#define ASYNCLIENTHTTP_CRYPTO_H

#include <string>
#include <optional>
#include <openssl/sha.h>
#include <openssl/evp.h>

std::string CalcSha512(const std::string& input);
char *base64(const std::string &input);
unsigned char *decode64(const std::string& input);


#endif

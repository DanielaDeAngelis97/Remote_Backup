//
// Created by daniela on 09/12/20.
//

#ifndef ASYNCLIENTHTTP_CRYPTO_H
#define ASYNCLIENTHTTP_CRYPTO_H
#include <string>
#include <optional>
#include <openssl/sha.h>
#include <openssl/evp.h>
static const int K_READ_BUF_SIZE{ 1024 * 16 };
std::string CalcSha512(std::string input);
unsigned char *decode64(std::string input);


#endif

#pragma once

#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/dh.h>
#include <openssl/rc4.h>
#include <openssl/rsa.h>
#include <stdint.h>

int s2n_crypto_free(uint8_t** data);

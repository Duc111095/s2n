#pragma once 

#include <openssl/bn.h>
#include <stdbool.h>
#include <stdint.h>

#include "crypto/s2n_openssl.h"

#define RSA_PSS_SIGN_VERIFY_RANDOM_BLOB_SIZE 32
#define RSA_PSS_SIGN_VERIFY_SIGNATURE_SIZE 256

#if defined(S2N_LIBCRYPTO_SUPPORTS_RSA_PSS_SIGNING) && OPENSSL_VERSION_NUMBER > 0x1010104fL
  #define RSA_PSS_CERTS_SUPPORTED 1
#else
  #define RSA_PSS_CERTS_SUPPORTED 0
#endif

bool s2n_is_rsa_pss_certs_supported();
bool s2n_is_rsa_pss_signing_supported();

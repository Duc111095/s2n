#pragma once

#if S2N_OPENSSL_VERSION_AT_LEAST(3, 0, 0)
#include <openssl/core_names.h>
#include <openssl/kdf.h>

#define S2N_OSSL_PARAM_BLOB(id, blob) \
  OSSL_PARAM_octet_string(id, blob->data, blob->size)
#define S2N_OSSL_PARAM_STR(id, cstr)  \
  OSSL_PARAM_utf8_string(id, cstr, strlen(cstr))
#define S2N_OSSL_PARAM_INT(id, val) \
  OSSL_PARAM_int(id, &val)

DEFINE_POINTER_CLEANUP_FUNC(EVP_KDF_CTX *, EVP_KDF_CTX_free);
DEFINE_POINTER_CLEANUP_FUNC(EVP_KDF *, EVP_KDF_free);

#endif

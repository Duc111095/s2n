#include "crypto/s2n_crypto.h"

#include <stdint.h>

#include <api/s2n.h>

#if defined(OPENSSL_IS_BORINGSSL) || defined(OPENSSL_IS_AWSLC)
  #include <openssl/mem.h>
#else
  #include <openssl/crypto.h>
#endif

int s2n_crypto_free(uint8_t **data) 
{
  if (*data != NULL) {
    OPENSSL_free(*data);
  }
  return S2N_SUCCESS;
}

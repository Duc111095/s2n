#pragma once

#include <openssl/x509v3.h>

#include "api/s2n.h"
#include "api/unstable/crl.h"
#include "utils/s2n_result.h"

struct s2n_x509_validator;

struct s2n_crl {
  X509_CRL *crl;
};

typedef enum {
  AWAITING_RESPONSE,
  FINISHED
} crl_lookup_callback_status;

struct s2n_crl_lookup {
  crl_lookup_callback_status status;
  X509* cert;
  uint16_t cert_idx;
  struct s2n_crl *crl;
};

S2N_RESULT s2n_crl_handle_lookup_callback_result(struct s2n_x509_validator *validator);
S2N_RESULT s2n_crl_invoke_lookup_callbacks(struct s2n_connection *conn, struct s2n_x509_validator *validator);
S2N_RESULT s2n_crl_get_crls_from_lookup_list(struct s2n_x509_validator *validator, STACK_OF(X509_CRL) *crl_stack);
int s2n_crl_ossl_verify_callback(int default_ossl_ret, X509_STORE_CTX *ctx);


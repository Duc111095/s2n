#pragma once

#include <openssl/asn1.h>
#include <openssl/x509.h>
#include <stdint.h>

#include "crypto/s2n_certificate.h"
#include "utils/s2n_blob.h"
#include "utils/s2n_safety.h"

#define S2N_MAX_ALLOWED_CERT_TRAILING_BYTES 3

DEFINE_POINTER_CLEANUP_FUNC(X509*, X509_free);

S2N_CLEANUP_RESULT s2n_openssl_x509_stack_pop_free(STACK_OF(X509) **cert_chain);
S2N_CLEANUP_RESULT s2n_openssl_asn1_time_free_pointer(ASN1_GENERALIZEDTIME **time);

S2N_RESULT s2n_openssl_x509_parse(struct s2n_blob *asn1der, X509 **cert_out);
S2N_RESULT s2n_openssl_x509_parse_without_length_validation(struct s2n_blob *asn1der, X509 **cert_out);
S2N_RESULT s2n_openssl_x509_get_cert_info(X509 *cert, struct s2n_cert_info *info);


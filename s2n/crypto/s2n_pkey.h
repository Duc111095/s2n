#pragma once

#include <openssl/evp.h>

#include "crypto/s2n_hash.h"
#include "crypto/s2n_signature.h"
#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

typedef enum {
  S2N_PKEY_TYPE_UNKNOWN = -1,
  S2N_PKEY_TYPE_RSA = 0,
  S2N_PKEY_TYPE_ECDSA,
  S2N_PKEY_TYPE_RSA_PSS,
  S2N_PKEY_TYPE_MLDSA,
  S2N_PKEY_TYPE_SENTINEL
} s2n_pkey_type;

struct s2n_pkey {
  EVP_PKEY *pkey;

  S2N_RESULT (*size)(const struct s2n_pkey *pkey, uint32_t *size_out);
  int (*sign)(const struct s2n_pkey *priv_key, s2n_signature_algorithm sig_alg,
      struct s2n_hash_state *digest, struct s2n_blob *signature);
  int (*verify)(const struct s2n_pkey *pub_key, s2n_signature_algorithm sig_alg,
      struct s2n_hash_state *digest, struct s2n_blob *signature);
  int (*encrypt)(const struct s2n_pkey *key, struct s2n_blob *in, struct s2n_blob *out);
  int (*decrypt)(const struct s2n_pkey *key, struct s2n_blob *in, struct s2n_blob *out);
};

int s2n_pkey_zero_init(struct s2n_pkey *pkey);
S2N_RESULT s2n_pkey_setup_for_type(struct s2n_pkey *pkey, s2n_pkey_type pkey_type);
int s2n_pkey_check_key_exists(const struct s2n_pkey *pkey);

S2N_RESULT s2n_pkey_size(const struct s2n_pkey *pkey, uint32_t *size_out);
int s2n_pkey_sign(const struct s2n_pkey *pkey, s2n_signature_algorithm sig_alg,
    struct s2n_hash_state *digest, struct s2n_blob *signature);
int s2n_pkey_verify(const struct s2n_pkey *pkey, s2n_signature_algorithm sig_alg,
    struct s2n_hash_state *digest, struct s2n_blob *signature);
int s2n_pkey_encrypt(const struct s2n_pkey *pkey, struct s2n_blob *in, struct s2n_blob *out);
int s2n_pkey_decrypt(const struct s2n_pkey *pkey, struct s2n_blob *in, struct s2n_blob *out);
int s2n_pkey_match(const struct s2n_pkey *pub_key, const struct s2n_pkey *priv_key);
int s2n_pkey_free(struct s2n_pkey *pkey);

S2N_RESULT s2n_pkey_init_hash(const struct s2n_pkey *pkey,
    s2n_signature_algorithm sig_alg, struct s2n_hash_state *hash);
S2N_RESULT s2n_asn1der_to_private_key(struct s2n_pkey *priv_key, struct s2n_blob *ans1der, int type_hint);
S2N_RESULT s2n_asn1der_to_public_key_and_type(struct s2n_pkey *pub_key, s2n_pkey_type *pkey_type, struct s2n_blob *asn1der);
S2N_RESULT s2n_pkey_get_type(EVP_PKEY *evp_pkey, s2n_pkey_type *pkey_type);
S2N_RESULT s2n_pkey_from_x509(X509 *cert, struct s2n_pkey *pub_key_out,
    s2n_pkey_type *pkey_type_out);


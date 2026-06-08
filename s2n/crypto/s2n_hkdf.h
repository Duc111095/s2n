#pragma once

#include <stdint.h>

#include "crypto/s2n_hmac.h"
#include "utils/s2n_blob.h"

#define S2N_MAX_HKDF_EXPAND_LABEL_LENGTH 249

int s2n_hkdf(struct s2n_hmac_state *hmac, s2n_hmac_algorithm alg, const struct s2n_blob *salt,
    const struct s2n_blob *key, const struct s2n_blob *info, struct s2n_blob *output);
int s2n_hkdf_extract(struct s2n_hmac_state *hmac, s2n_hmac_algorithm alg, const struct s2n_blob *salt, const struct s2n_blob *key, struct s2n_blob *pseudo_rand_key);
int s2n_hkdf_expand_label(struct s2n_hmac_state *hmac, s2n_hmac_algorithm alg, const struct s2n_blob *secret, const struct s2n_blob *label, const struct s2n_blob *context, struct s2n_blob *output);
bool s2n_libcrypto_supports_hkdf();


#pragma once

#include "crypto/s2n_pkey.h"
#include "utils/s2n_result.h"

#if S2N_LIBCRYPTO_SUPPORTS_MLDSA
  #define S2N_NID_MLDSA44 NID_MLDSA44
  #define S2N_NID_MLDSA65 NID_MLDSA65
  #define S2N_NID_MLDSA87 NID_MLDSA87
#else
  #define S2N_NID_MLDSA44 NID_undef
  #define S2N_NID_MLDSA65 NID_undef
  #define S2N_NID_MLDSA87 NID_undef
#endif

#define S2N_MLDSA_MAX_PUB_KEY_SIZE 2592

bool s2n_mldsa_is_supported();
S2N_RESULT s2n_mldsa_init_mu_hash(struct s2n_hash_state *state, const struct s2n_pkey *pub_key);



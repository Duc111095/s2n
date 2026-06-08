#pragma once

#include "tls/s2n_kem.h"

int s2n_evp_kem_generate_keypair(IN const struct s2n_kem *kem, OUT uint8_t *public_key, OUT uint8_t *private_key);
int s2n_evp_kem_encapsulate(IN const struct s2n_kem *kem, OUT uint8_t *ciphertext, OUT uint8_t *shared_secret, IN const uint8_t *public_key);
int s2n_evp_kem_decapsulate(IN const struct s2n_kem *kem, OUT uint8_t *shared_secret, IN const uint8_t *ciphertext, IN const uint8_t *private_key);



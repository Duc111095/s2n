#pragma once

#include <stdint.h>

struct s2n_certificate_key {
  const char *name;
  uint16_t public_key_libcrypto_nid;

  uint16_t bits;
};

struct s2n_certificate_key_preferences {
  uint8_t count;
  const struct s2n_certificate_key *const *certificate_keys;
};

extern const struct s2n_certificate_key s2n_rsa_rsae_1024;
extern const struct s2n_certificate_key s2n_rsa_rsae_2048;
extern const struct s2n_certificate_key s2n_rsa_rsae_3072;
extern const struct s2n_certificate_key s2n_rsa_rsae_4096;

extern const struct s2n_certificate_key s2n_rsa_pss_1024;
extern const struct s2n_certificate_key s2n_rsa_pss_2048;
extern const struct s2n_certificate_key s2n_rsa_pss_3072;
extern const struct s2n_certificate_key s2n_rsa_pss_4096;

extern const struct s2n_certificate_key s2n_ec_p256;
extern const struct s2n_certificate_key s2n_ec_p384;
extern const struct s2n_certificate_key s2n_ec_p521;

extern struct s2n_certificate_key_preferences s2n_certificate_key_preferences_20250429;
extern struct s2n_certificate_key_preferences s2n_certificate_key_preferences_20260219;
extern struct s2n_certificate_key_preferences s2n_certificate_key_preferences_20260220;

#pragma once

#include <strings.h>

#include "api/s2n.h"
#include "crypto/s2n_ecc_evp.h"
#include "crypto/s2n_hash.h"
#include "crypto/s2n_signature.h"

struct s2n_signature_scheme {
  uint16_t iana_value;
  const char* name;
  s2n_hash_algorithm hash_alg;
  s2n_signature_algorithm sig_alg;
  uint8_t minimum_protocol_version;
  uint8_t maximum_protocol_version;
  uint16_t libcrypto_nid;
  struct s2n_ecc_named_curve const *signature_curve;

  const char *legacyu_name;
  const char *tls13_name;
};

struct s2n_signature_preferences {
  uint8_t count;
  const struct s2n_signature_scheme *const *signature_schemes;
};

extern const struct s2n_signature_scheme s2n_null_sig_scheme;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_md5_sha1;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_sha1;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_sha224;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_sha256;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_sha384;
extern const struct s2n_signature_scheme s2n_rsa_pkcs1_sha512;

extern const struct s2n_signature_scheme s2n_ecdsa_sha1;
extern const struct s2n_signature_scheme s2n_ecdsa_sha224;
extern const struct s2n_signature_scheme s2n_ecdsa_sha256;
extern const struct s2n_signature_scheme s2n_ecdsa_sha384;
extern const struct s2n_signature_scheme s2n_ecdsa_sha512;

extern const struct s2n_signature_scheme s2n_rsa_pss_pss_sha256;
extern const struct s2n_signature_scheme s2n_rsa_pss_pss_sha384;
extern const struct s2n_signature_scheme s2n_rsa_pss_pss_sha512;
extern const struct s2n_signature_scheme s2n_rsa_pss_rsae_sha256;
extern const struct s2n_signature_scheme s2n_rsa_pss_rsae_sha384;
extern const struct s2n_signature_scheme s2n_rsa_pss_rsae_sha512;

extern const struct s2n_signature_scheme s2n_mldsa44;
extern const struct s2n_signature_scheme s2n_mldsa65;
extern const struct s2n_signature_scheme s2n_mldsa87;

extern const struct s2n_signature_preferences s2n_signature_preferences_20250512;
extern const struct s2n_signature_preferences s2n_signature_preferences_20240501;
extern const struct s2n_signature_preferences s2n_signature_preferences_20230317;
extern const struct s2n_signature_preferences s2n_signature_preferences_20140601;
extern const struct s2n_signature_preferences s2n_signature_preferences_20200207;
extern const struct s2n_signature_preferences s2n_signature_preferences_20200207_no_sha1;
extern const struct s2n_signature_preferences s2n_signature_preferences_20201021;
extern const struct s2n_signature_preferences s2n_signature_preferences_20210816;
extern const struct s2n_signature_preferences s2n_signature_preferences_20240521;
extern const struct s2n_signature_preferences s2n_signature_preferences_20250429;
extern const struct s2n_signature_preferences s2n_signature_preferences_20250820;
extern const struct s2n_signature_preferences s2n_signature_preferences_20250821;
extern const struct s2n_signature_preferences s2n_signature_preferences_20251113;
extern const struct s2n_signature_preferences s2n_signature_preferences_20260219;
extern const struct s2n_signature_preferences s2n_signature_preferences_20260220;
extern const struct s2n_signature_preferences s2n_signature_preferences_default_fips;
extern const struct s2n_signature_preferences s2n_signature_preferences_null;
extern const struct s2n_signature_preferences s2n_signature_preferences_test_all_fips;
extern const struct s2n_signature_preferences s2n_signature_preferences_all;
extern const struct s2n_signature_preferences s2n_signature_preferences_20250813;

extern const struct s2n_signature_preferences s2n_certificate_signature_preferences_20260220;
extern const struct s2n_signature_preferences s2n_certificate_signature_preferences_20251113;
extern const struct s2n_signature_preferences s2n_certificate_signature_preferences_20250512;
extern const struct s2n_signature_preferences s2n_certificate_signature_preferences_20250429;
extern const struct s2n_signature_preferences s2n_certificate_signature_preferences_20201110;

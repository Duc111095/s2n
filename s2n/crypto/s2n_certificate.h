#pragma once

#include <openssl/x509.h>
#include <stdint.h>

#include "api/s2n.h"
#include "crypto/s2n_pkey.h"
#include "stuffer/s2n_stuffer.h"

#define S2N_CERT_TYPE_COUNT S2N_PKEY_TYPE_SENTINEL

struct s2n_cert_info {
  int signature_nid;
  int signature_digest_nid;
  int public_key_nid;
  int public_key_bits;
  bool self_signed;
};

struct s2n_cert {
  s2n_pkey_type pkey_type;
  s2n_cert_public_key public_key;
  struct s2n_cert_info info;
  struct s2n_blob raw;
  struct s2n_cert *next;
};

struct s2n_cert_chain {
  uint32_t chain_size;
  struct s2n_cert *head;
};

struct s2n_cert_chain_and_key {
  struct s2n_cert_chain *cert_chain;
  s2n_cert_private_key *private_key;
  struct s2n_blob oscp_status;
  struct s2n_blob sct_list;

  struct s2n_array *san_names;
  struct s2n_array *cn_names;
  void *context;
};

struct certs_by_type {
  struct s2n_cert_chain_and_key *certs[S2N_CERT_TYPE_COUNT];
};

int s2n_cert_chain_and_key_load_cns(struct s2n_cert_chain_and_key *chain_and_key, X509 *x509_cert);
int s2n_cert_chain_and_key_load_sans(struct s2n_cert_chain_and_key *chain_and_key, X509 *x509_cert);
int s2n_cert_chain_and_key_matches_dns_name(const struct s2n_cert_chain_and_key *chain_and_key, const struct s2n_blob *dns_name);

S2N_CLEANUP_RESULT s2n_cert_chain_and_key_ptr_free(struct s2n_cert_chain_and_key **cert_and_key);
int s2n_cert_set_cert_type(struct s2n_cert *cert, s2n_pkey_type pkey_type);
int s2n_send_cert_chain(struct s2n_connection *conn, struct s2n_stuffer *out, struct s2n_cert_chain_and_key *chain_and_key);
int s2n_send_empty_cert_chain(struct s2n_stuffer *out);
int s2n_create_cert_chain_from_stuffer(struct s2n_cert_chain *cert_chain_out, struct s2n_stuffer *chain_in_stuffer);
int s2n_cert_chain_and_key_set_cert_chain_bytes(struct s2n_cert_chain_and_key *cert_and_key, uint8_t *cert_chain_pem, uint32_t cert_chain_len);
int s2n_cert_chain_and_key_set_private_key_bytes(struct s2n_cert_chain_and_key *cert_and_key, uint8_t *private_key_pem, uint32_t private_key_len);
int s2n_cert_chain_and_key_set_cert_chain(struct s2n_cert_chain_and_key *cert_and_key, const char *cert_chain_pem);
int s2n_cert_chain_and_key_set_private_key(struct s2n_cert_chain_and_key *cert_and_key, const char *private_key_pem);
s2n_pkey_type s2n_cert_chain_and_key_get_pkey_type(struct s2n_cert_chain_and_key *chain_and_key);
int s2n_cert_chain_get_length(const struct s2n_cert_chain_and_key *chain_and_key, uint32_t *cert_length);
int s2n_cert_chain_get_cert(const struct s2n_cert_chain_and_key *chain_and_key, struct s2n_cert **out_cert, const uint32_t cert_idx);
int s2n_cert_get_der(const struct s2n_cert *cert, const uint8_t **out_cert_der, uint32_t *cert_length);
int s2n_cert_chain_free(struct s2n_cert_chain *cert_chain);
int s2n_cert_get_x509_extension_value_length(struct s2n_cert *cert, const uint8_t *oid, uint32_t *ext_value_len);
int s2n_cert_get_x509_extension_value(struct s2n_cert *cert, const uint8_t *oid, uint8_t *ext_value, uint32_t *ext_value_len, bool *critical);
int s2n_cert_get_utf8_string_from_extension_data_length(const uint8_t* extension_data, uint32_t extension_len, uint32_t *utf8_str_len);
int s2n_cert_get_utf8_string_from_extension_data(const uint8_t *extension_data, uint32_t extension_len, uint8_t *out_data, uint32_t *out_len);


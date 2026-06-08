#pragma once

#include <stdint.h>

#include "api/s2n.h"
#include "stuffer/s2n_stuffer.h"
#include "tls/extensions/s2n_extension_list.h"
#include "tls/s2n_crypto_constants.h"
#include "utils/s2n_array.h"

struct s2n_client_hello {
  struct s2n_blob raw_message;
  uint8_t random[S2N_TLS_RANDOM_DATA_LEN];

  s2n_parsed_extensions_list extensions;
  struct s2n_blob cipher_suites;
  struct s2n_blob session_id;
  struct s2n_blob compression_methods;

  uint8_t legacy_version;
  uint8_t legacy_record_version;
  unsigned int record_version_recorded : 1;

  unsigned int callback_invoked : 1;
  unsigned int callback_async_blocked : 1;
  unsigned int callback_async_done : 1;

  unsigned int parsed : 1;
  unsigned int sslv2 : 1;
  unsigned int alloced : 1;
};

int s2n_client_hello_free_raw_message(struct s2n_client_hello *client_hello);

struct s2n_client_hello *s2n_connection_get_client_hello(struct s2n_connection *conn);

ssize_t s2n_client_hello_get_raw_message_length(struct s2n_client_hello *ch);
ssize_t s2n_client_hello_get_raw_message(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

ssize_t s2n_client_hello_get_cipher_suites_length(struct s2n_client_hello *ch);
ssize_t s2n_client_hello_get_cipher_suites(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

int s2n_client_hello_get_parsed_extensions(s2n_tls_extension_type extension_type, s2n_parsed_extensions_list *parsed_extension_list, s2n_parsed_extension **parsed_extension);
ssize_t s2n_client_hello_get_extensions_length(struct s2n_client_hello *ch);
ssize_t s2n_client_hello_get_extensions(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);

int s2n_client_hello_get_random(struct s2n_client_hello *ch, uint8_t *out, uint32_t max_length);


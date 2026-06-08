#pragma once

#include <stdbool.h>

#include "stuffer/s2n_stuffer.h"
#include "tls/s2n_tls_parameters.h"

#define S2N_EXTENSION_TYPE_FIELD_LENGTH 2
#define S2N_EXTENSION_LENGTH_FIELD_LENGTH 2
#define S2N_EXTENSION_HEADER_LENGTH (S2N_EXTENSION_TYPE_FIELD_LENGTH + S2N_EXTENSION_LENGTH_FIELD_LENGTH)

#define S2N_SUPPORTED_EXTENSIONS_COUNT (sizeof(s2n_supported_extensions) / sizeof(s2n_supported_extensions[0]))

#define S2N_SUPPORTED_EXTENSIONS_BITFIELD_LEN ((S2N_SUPPORTED_EXTENSIONS_COUNT + CHAR_BIT - 1) / CHAR_BIT)

struct s2n_connection;
typedef struct  {
  uint16_t iana_value;
  unsigned is_response : 1;
  uint16_t minimum_version;

  int (*send)(struct s2n_connection *conn, struct s2n_stuffer *out);
  int (*recv)(struct s2n_connection *conn, struct s2n_stuffer *in);

  bool (*should_sent)(struct s2n_connection *conn);
  int (*if_missing)(struct s2n_connection *conn);
} s2n_extension_type;

static const uint16_t s2n_supported_extensions[] = {
  TLS_EXTENSION_RENEGOTIATION_INFO,
  TLS_EXTENSION_PQ_KEM_PARAMETERS,
  TLS_EXTENSION_SERVER_NAME,
  TLS_EXTENSION_MAX_FRAG_LEN,
  TLS_EXTENSION_STATUS_REQUEST,
  TLS_EXTENSION_SUPPORTED_GROUPS, 
  TLS_EXTENSION_EC_POINT_FORMATS,
  TLS_EXTENSION_SIGNATURE_ALGORITHMS,
  TLS_EXTENSION_ALPN,
  TLS_EXTENSION_SCT_LIST,
  TLS_EXTENSION_SESSION_TICKET,
  TLS_EXTENSION_SUPPORTED_VERSIONS,
  TLS_EXTENSION_KEY_SHARE,
  TLS_EXTENSION_COOKIE,
  TLS_EXTENSION_QUIC_TRANSPORT_PARAMETERS,
  TLS_EXTENSION_PSK_KEY_EXCHANGE_MODES,
  TLS_EXTENSION_PRE_SHARED_KEY,
  TLS_EXTENSION_EARLY_DATA,
  TLS_EXTENSION_EMS,
  TLS_EXTENSION_NPN,
  TLS_EXTENSION_CERT_AUTHORITIES,
};

typedef char s2n_extension_bitfield[S2N_SUPPORTED_EXTENSIONS_BITFIELD_LEN];

typedef uint8_t s2n_extension_type_id;
extern const s2n_extension_type_id s2n_unsupported_extension;

int s2n_extension_send(const s2n_extension_type *extension_type, struct s2n_connection *conn, struct s2n_stuffer *out);
int s2n_extension_recv(const s2n_extension_type *extension_type, struct s2n_connection *conn, struct s2n_stuffer *in);
int s2n_extension_is_missing(const s2n_extension_type *extension_type, struct s2n_connection *conn);

int s2n_extension_supported_iana_value_to_id(const uint16_t iana_value, s2n_extension_type_id *internal_id);

int s2n_extension_type_init();

int s2n_extension_send_unimplemented(struct s2n_connection *conn, struct s2n_stuffer *out);
int s2n_extension_send_noop(struct s2n_connection *conn, struct s2n_stuffer *out);

int s2n_extension_recv_unimplemented(struct s2n_connection *conn, struct s2n_stuffer *in);
int s2n_extension_recv_noop(struct s2n_connection *conn, struct s2n_stuffer *out);

bool s2n_extension_always_send(struct s2n_connection *conn);
bool s2n_extension_never_send(struct s2n_connection *conn);
bool s2n_extension_send_if_tls13_connection(struct s2n_connection *conn);

int s2n_extension_error_if_missing(struct s2n_connection *conn);
int s2n_extension_noop_if_missing(struct s2n_connection *conn);

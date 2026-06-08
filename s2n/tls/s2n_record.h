#pragma once

#include <stdint.h>

#include "crypto/s2n_hmac.h"
#include "stuffer/s2n_stuffer.h"

#define S2N_TLS_CONTENT_TYPE_LENGTH 1
#define S2N_TLS_SSLV2_HEADER_FLAG (0x80)
#define S2N_TLS_SSLV2_HEADER_FLAG_UINT16 (S2N_TLS_SSLV2_HEADER_FLAG << 8)

#define S2N_TLS_RECORD_HEADER_LENGTH (S2N_TLS_CONTENT_TYPE_LENGTH + S2N_TLS_PROTOCOL_VERSION_LEN + 2)

#define S2N_TLS_MAXIMUM_FRAGMENT_LENGTH (1 << 14)

#define S2N_TLS12_ENCRYPTION_OVERHEAD_SIZE 1024
#define S2N_TLS12_MAX_RECORD_LEN_FOR(frag)  \
  ((frag) + S2N_TLS12_ENCRYPTION_OVERHEAD_SIZE + S2N_TLS_RECORD_HEADER_LENGTH)
#define S2N_TLS12_MAXIMUM_RECORD_LENGTH S2N_TLS12_MAX_RECORD_LEN_FOR(S2N_TLS_MAXIMUM_FRAGMENT_LENGTH)

#define S2N_TLS13_ENCRYPTION_OVERHEAD_SIZE 255
#define S2N_TLS13_MAX_RECORD_LEN_FOR(frag) ((frag) + S2N_TLS_CONTENT_TYPE_LENGTH \
    + S2N_TLS13_ENCRYPTION_OVERHEAD_SIZE \
    + S2N_TLS_RECORD_HEADER_LENGTH)
#define S2N_TLS13_MAXIMUM_RECORD_LENGTH S2N_TLS13_MAX_RECORD_LEN_FOR(S2N_TLS_MAXIMUM_FRAGMENT_LENGTH)

#define S2N_TLS_MAX_RECORD_LEN_FOR(frag) S2N_TLS12_MAX_RECORD_LEN_FOR(frag)
#define S2N_TLS_MAXIMUM_RECORD_LENGTH S2N_TLS_MAX_RECORD_LENF_FOR(S2N_TLS_MAXIMUM_FRAGMENT_LENGTH)

S2N_RESULT s2n_record_max_write_size(struct s2n_connection *conn, uint16_t max_fragment_size, uint16_t *max_record_size);
S2N_RESULT s2n_record_max_write_payload_size(struct s2n_connection *conn, uint16_t *max_fragment_size);
S2N_RESULT s2n_record_min_write_payload_size(struct s2n_connection* conn, uint16_t *payload_size);
S2N_RESULT s2n_record_write(struct s2n_connection *conn, uint8_t content_type, struct s2n_blob *in);
int s2n_record_writev(struct s2n_connection *conn, uint8_t content_type, const struct iovec *in, int in_count, size_t offs, size_t to_write);
int s2n_record_parse(struct s2n_connection *conn);
int s2n_record_header_parse(struct s2n_connection *conn, uint8_t *content_type, uint16_t *fragment_length);
int s2n_tls13_parse_record_type(struct s2n_stuffer *stuffer, uint8_t *record_type);
int s2n_sslv2_record_header_parse(struct s2n_connection *conn, uint8_t record_type, uint8_t* client_protocol_version, uint16_t *fragment_length);
int s2n_verify_cbc(struct s2n_connection *conn, struct s2n_hmac_state *hmac, struct s2n_blob *decrypted);
S2N_RESULT s2n_aead_aad_init(const struct s2n_connection *conn, uint8_t *sequence_number, uint8_t content_type, uint16_t record_length, struct s2n_blob *ad);
S2N_RESULT s2n_tls13_aead_aad_init(uint16_t record_length, uint8_t tag_length, struct s2n_blob *ad);
S2N_RESULT s2n_record_wipe(struct s2n_connection *conn);


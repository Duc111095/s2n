#pragma once
#include "api/s2n.h"

S2N_API int s2n_config_enable_quic(struct s2n_config *config);
S2N_API int s2n_connection_enable_quic(struct s2n_connection *conn);
S2N_API bool s2n_connection_is_quic_enabled(struct s2n_connection *conn);
S2N_API bool s2n_connection_are_session_tickets_enabled(struct s2n_connection *conn);

S2N_API int s2n_connection_set_quic_transport_parameters(struct s2n_connection *conn,
    const uint8_t *data_buffer, uint16_t data_len);

S2N_API int s2n_connection_get_quic_transport_parameters(struct s2n_connection *conn,
    const uint8_t **data_buffer, uint16_t *data_len);

typedef enum {
  S2N_CLIENT_EARLY_TRAFFIC_SECRET = 0,
  S2N_CLIENT_HANDSHAKE_TRAFFIC_SECRET,
  S2N_SERVER_HANDSHAKE_TRAFFIC_SECRET,
  S2N_CLIENT_APPLICATION_TRAFFIC_SECRET,
  S2N_SERVER_APPLICATION_TRAFFIC_SECRET,
  S2N_EXPORTER_SECRET,
} s2n_secret_type_t;

typedef int (*s2n_secret_cb)(void *context, struct s2n_connection *conn,
    s2n_secret_type_t secret_type, uint8_t* secret, uint8_t secret_size);

S2N_API int s2n_connection_set_secret_callback(struct s2n_connection *conn, s2n_secret_cb cb_func, void *ctx);

S2N_API int s2n_error_get_alert(int error, uint8_t *alert);

S2N_API int s2n_recv_quic_post_handshake_message(struct s2n_connection *conn, s2n_blocked_status *blocked);

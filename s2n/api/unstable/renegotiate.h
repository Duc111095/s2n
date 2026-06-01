#pragma once
#include "s2n.h"

extern const s2n_blocked_status S2N_BLOCKED_ON_APPLICATION_DATA;

/**
 * Indicates how a renegotiation request should be handled.
 */
typedef enum {
	S2N_RENEGOTIATE_IGNORE = 0,
	S2N_RENEGOTIATE_REJECT,
	S2N_RENEGOTIATE_ACCEPT,
} s2n_renegotiate_response;

typedef int (*s2n_renegotiate_request_cb)(struct s2n_connection* conn, void* context, s2n_renegotiate_response* response);

S2N_API int s2n_config_set_renegotiate_request_cb(struct s2n_config* config, s2n_renegotiate_request_cb callback, void* context);

S2N_API int s2n_renegotiate_wipe(struct s2n_connection* conn);

S2N_API int s2n_renegotiate(struct s2n_connection* conn, uint8_t* app_data_buf, ssize_t app_data_buf_size,
	ssize_t* app_data_size, s2n_blocked_status* blocked);
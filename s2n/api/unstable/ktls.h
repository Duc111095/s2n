#pragma once

#ifndef _WIN32
#include "s2n.h"

S2N_API int s2n_connection_ktls_enable_send(struct s2n_connection* conn);

S2N_API int s2n_connection_ktls_enable_recv(struct s2n_connection* conn);

S2N_API int s2n_config_ktls_enable_unsafe_tls13(struct s2n_config* config);

S2N_API int s2n_connection_get_key_update_counts(struct s2n_connection* conn,
	uint8_t* send_key_updates, uint8_t* recv_key_updates);

S2N_API int s2n_sendfile(struct s2n_connection* conn, int fd, off_t offset, size_t count,
	size_t* bytes_written, s2n_blocked_status* blocked);

#endif
#pragma once

#include "stuffer/s2n_stuffer.h"
#include "tls/extensions/s2n_extension_type.h"
#include "tls/s2n_connection.h"

extern const s2n_extension_type s2n_cert_status_extension;

int s2n_cert_status_send(struct s2n_connection *conn, struct s2n_stuffer *out);
int s2n_cert_status_recv(struct s2n_connection *conn, struct s2n_stuffer *in);

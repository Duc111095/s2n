#pragma once

#include "tls/s2n_connection.h"

#define S2N_KEY_UPDATE_MESSAGE_SIZE 5
#define S2N_KEY_UPDATE_LENGTH 1

typedef enum {
  SENDING = 0,
  RECEIVING
} keyupdate_status;

int s2n_key_update_recv(struct s2n_connection *conn, struct s2n_stuffer *request);
int s2n_key_update_send(struct s2n_connection *conn, s2n_blocked_status *blocked);
int s2n_key_update_write(struct s2n_blob *out);

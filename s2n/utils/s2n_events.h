#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "api/s2n.h"
#include "api/unstable/events.h"
#include "utils/s2n_result.h"

S2N_RESULT s2n_event_handshake_populate(struct s2n_connection* conn, struct s2n_event_handshake* event);
S2N_RESULT s2n_event_handshake_send(struct s2n_connection* conn, struct s2n_event_handshake* event);

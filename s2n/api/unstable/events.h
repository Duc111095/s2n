#pragma once

#include <s2n.h>
#include <stdint.h>

#define HANDSHAKE_EVENT_SENT UINT64_C(1) << 63

struct s2n_event_handshake {
	int protocol_version;
	const char* cipher;
	const char* group;
	uint64_t handshake_time_ns;
	uint64_t handshake_start_ns;
	uint64_t handshake_end_ns;
};

typedef void (*s2n_event_on_handshake_cb)(struct s2n_connection* conn, void* subscriber, struct s2n_event_handshake* event);

S2N_API extern int s2n_config_set_subscriber(struct s2n_config* config, void* subscriber);

S2N_API extern int s2n_config_set_handshake_event(struct s2n_config* config, s2n_event_on_handshake_cb callback);
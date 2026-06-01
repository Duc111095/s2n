#pragma once

#include <s2n.h>

struct s2n_async_offload_op;

typedef enum {
	S2N_ASYNC_OFFLOAD_PKEY_VERIFY = 0x01,
	S2N_ASYNC_OFFLOAD_ALLOW_ALL = 0x7FFFFFFF,
} s2n_async_offload_op_type;

typedef int (*s2n_async_offload_cb)(struct s2n_connection* conn, struct s2n_async_offload_op* op, void* ctx);

S2N_API extern int s2n_config_set_async_offload_callback(struct s2n_config* config, uint32_t allow_list,
	s2n_async_offload_cb fn, void* ctx);

S2N_API extern int s2n_async_offload_op_perform(struct s2n_async_offload_op* op);
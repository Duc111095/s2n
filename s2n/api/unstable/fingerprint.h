#pragma once

#include <s2n.h>

typedef enum {
	S2N_FINGERPRINT_JA3,
	S2N_FINGERPRINT_JA4
} s2n_fingerprint_type;

struct s2n_fingerprint;

S2N_API struct s2n_fingerprint* s2n_fingerprint_new(s2n_fingerprint_type type);

S2N_API int s2n_fingerprint_free(struct s2n_fingerprint** fingerprint);

S2N_API int s2n_fingerprint_wipe(struct s2n_fingerprint* fingerprint);

S2N_API int s2n_fingerprint_set_client_hello(struct s2n_fingerprint* fingerprint, struct s2n_client_hello* ch);

S2N_API int s2n_fingerprint_get_hash_size(const struct s2n_fingerprint* fingerprint, uint32_t* size);

S2N_API int s2n_fingerprint_get_hash(struct s2n_fingerprint* fingerprint,
	uint32_t max_output_size, uint8_t* output, uint32_t* output_size);

S2N_API int s2n_fingerprint_get_raw_size(const struct s2n_fingerprint* fingerprint, uint32_t* size);

S2N_API int s2n_fingerprint_get_raw(struct s2n_fingerprint* fingerprint,
	uint32_t max_output_size, uint8_t* output, uint32_t* output_size);

S2N_API int s2n_client_hello_get_fingerprint_hash(struct s2n_client_hello* ch,
	s2n_fingerprint_type type, uint32_t max_hash_size,
	uint8_t* hash, uint32_t* hash_size, uint32_t* str_size);

S2N_API int s2n_client_hello_get_fingerprint_string(struct s2n_client_hello* ch,
	s2n_fingerprint_type type, uint32_t max_size,
	uint8_t* output, uint32_t* output_size);
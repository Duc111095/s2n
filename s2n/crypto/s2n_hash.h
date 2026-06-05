#pragma once

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdint.h>

#include "crypto/s2n_evp.h"

#define S2N_MAX_DIGEST_LEN SHA512_DIGEST_LENGTH

typedef enum {
	S2N_HASH_NONE = 0,
	S2N_HASH_MD5,
	S2N_HASH_SHA1,
	S2N_HASH_SHA224,
	S2N_HASH_SHA256,
	S2N_HASH_SHA384,
	S2N_HASH_SHA512,
	S2N_HASH_MD5_SHA1,
	S2N_HASH_SHAKE256_64,
	S2N_HASH_ALGS_COUNT
} s2n_hash_algorithm;

struct s2n_hash_evp_digest {
	struct s2n_evp_digest evp;
	struct s2n_evp_digest evp_md5_secondary;
};

struct s2n_hash_state {
	const struct s2n_hash* hash_impl;
	s2n_hash_algorithm alg;
	uint8_t is_ready_for_input;
	uint64_t currently_in_hash;
	union {
		struct s2n_hash_evp_digest high_level;
	} digest;
};

struct s2n_hash {
	int (*alloc)(struct s2n_hash_state* state);
	int (*init)(struct s2n_hash_state* state, s2n_hash_algorithm alg);
	int (*update)(struct s2n_hash_state* state, const void* data, uint32_t size);
	int (*digest)(struct s2n_hash_state* state, void* out, uint32_t size);
	int (*copy)(struct s2n_hash_state* to, struct s2n_hash_state* from);
	int (*reset)(struct s2n_hash_state* state);
	int (*free)(struct s2n_hash_state* state);
};

S2N_RESULT s2n_hash_algorithms_init();
S2N_RESULT s2n_hash_algorithms_cleanup();
bool s2n_hash_use_custom_md5_sha1();
bool s2n_hash_supports_shake();
const EVP_MD* s2n_hash_alg_to_evp_md(s2n_hash_algorithm alg);
int s2n_hash_digest_size(s2n_hash_algorithm alg, uint8_t* out);
bool s2n_hash_is_available(s2n_hash_algorithm alg);
int s2n_hash_is_ready_for_input(struct s2n_hash_state* state);
int s2n_hash_new(struct s2n_hash_state* state)
S2N_RESULT s2n_hash_state_validate(struct s2n_hash_state* state);
int s2n_hash_init(struct s2n_hash_state* state, s2n_hash_algorithm alg);
int s2n_hash_update(struct s2n_hash_state* state, const void* data, uint32_t size);
int s2n_hash_digest(struct s2n_hash_state* state, void* out, uint32_t size);
int s2n_hash_copy(struct s2n_hash_state* to, struct s2n_hash_state* from);
int s2n_hash_reset(struct s2n_hash_state* state);
int s2n_hash_free(struct s2n_hash_state* state);
int s2n_hash_get_currently_in_hash_total(struct s2n_hash_state* state, uint64_t* out);

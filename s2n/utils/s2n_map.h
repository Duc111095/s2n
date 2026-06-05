#pragma once

#include <string.h>

#include "crypto/s2n_hash.h"
#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

struct s2n_map;
struct s2n_map_iterator {
	const struct s2n_map* map;
	uint32_t current_index;
	bool consumed;
};

struct s2n_map* s2n_map_new();
struct s2n_map* s2n_map_new_with_initial_capacity(uint32_t capacity);
S2N_RESULT s2n_map_add(struct s2n_map* map, struct s2n_blob* key, struct s2n_blob* value);
S2N_RESULT s2n_map_put(struct s2n_map* map, struct s2n_blob* key, struct s2n_blob* value);
S2N_RESULT s2n_map_complete(struct s2n_map* map);
S2N_RESULT s2n_map_unlock(struct s2n_map* map);
S2N_RESULT s2n_map_lookup(const struct s2n_map* map, struct s2n_blob* key, struct s2n_blob* value, bool* key_found);
S2N_RESULT s2n_map_free(struct s2n_map* map);
S2N_RESULT s2n_map_size(struct s2n_map* map, uint32_t* size);

S2N_RESULT s2n_map_iterator_init(struct s2n_map_iterator* iter, const struct s2n_map* map);
S2N_RESULT s2n_map_iterator_next(struct s2n_map_iterator* iter, struct s2n_blob* value);
bool s2n_map_iterator_has_next(const struct s2n_map_iterator* iter);
#pragma once

#include "utils/s2n_map.h"

struct s2n_map_entry {
	struct s2n_blob key;
	struct s2n_blob value;
};

struct s2n_map {
	uint32_t capacity;
	uint32_t size;
	int immutable;
	struct s2n_map_entry* table;
};
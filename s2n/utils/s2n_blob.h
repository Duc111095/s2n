#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "utils/s2n_result.h"

struct s2n_blob {
	uint8_t* data;
	uint32_t size;
	uint32_t allocated;
	unsigned growable : 1;
};

bool s2n_blob_is_growable(const struct s2n_blob* b);
S2N_RESULT s2n_blob_validate(const struct s2n_blob* b);
int S2N_RESULT_MUST_USE s2n_blob_init(struct s2n_blob* b, uint8_t* data, uint32_t size);
int s2n_blob_zero(struct s2n_blob* b);
int S2N_RESULT_MUST_USE s2n_blob_char_to_lower(struct s2n_blob* b);
int S2N_RESULT_MUST_USE s2n_blob_slice(const struct s2n_blob* b, struct s2n_blob* slice, uint32_t offset, uint32_t size);

#define s2n_stack_blob(name, requested_size, maximum)	\
	size_t name##_requested_size = (requested_size);	\
	uint8_t name##_buf[(maximum)] = {0};	\
	POSIX_ENSURE_LTE(name##_requested_size, (maximum));	\
	struct s2n_blob_name = {0};	\
	POSIX_GUARD(s2n_blob_init(&name, name##_buf, name##_requested_size))

#define RESULT_STACK_BLOB(name, requested_size, maximum)	\
	size_t name##_requested_size = (requested_size);	\
	uint8_t name##_buf[(maximum)] = {0};	\
	RESULT_ENSURE_LTE(name##_requested_size, (maximum));	\
	struct s2n_blob_name = {0};	\
	RESULT_GUARD_POSIX(s2n_blob_init(&name, name##_buf, name##_requested_size))

#define S2N_BLOB_LABEL(name, str) \
	static uint8_t name##_data[] = str;	\
	const struct s2n_blob name = {.data = name##_data, .size = sizeof(name##_data) - 1};


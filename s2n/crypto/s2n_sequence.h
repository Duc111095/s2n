#pragma once

#include "utils/s2n_blob.h"

int s2n_increment_sequence_number(struct s2n_blob* sequence_number);
int s2n_sequence_number_to_uint64(struct s2n_blob* sequence_number, uint64_t *output);

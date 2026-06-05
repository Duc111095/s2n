#pragma once

#include <signal.h>

#include "utils/s2n_result.h"

typedef struct {
	sig_atomic_t val;
} s2n_atomic_flag;

S2N_RESULT s2n_atomic_init();
void s2n_atomic_flag_set(s2n_atomic_flag* var);
void s2n_atomic_flag_clear(s2n_atomic_flag* var);
bool s2n_atomic_flag_test(s2n_atomic_flag* var);

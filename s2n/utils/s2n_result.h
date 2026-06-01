#pragma once

#include "api/s2n.h"

typedef struct {
	int __error_signal;
} s2n_result;

#define S2N_RESULT_OK ((s2n_result){ S2N_SUCCESS })
#define S2N_RESULT_ERROR ((s2n_result){S2N_FAILURE})

#if defined(__clang__) || defined(__GNUC__)
	#define S2N_RESULT_MUST_USE __attribute__((warn_unused_result))
#else
	#define S2N_RESULT_MUST_USE
#endif

S2N_RESULT_MUST_USE static inline bool s2n_result_is_ok(s2n_result result) {
	return result.__error_signal == S2N_SUCCESS;
}

S2N_RESULT_MUST_USE static inline bool s2n_result_is_error(s2n_result result) {
	return result.__error_signal == S2N_FAILURE;
}

static inline void s2n_result_ignore(s2n_result result) {

}

#define S2N_RESULT S2N_RESULT_MUST_USE s2n_result

#define S2N_CLEANUP_RESULT s2n_result
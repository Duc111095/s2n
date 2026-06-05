#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error/s2n_errno.h"
#include "utils/s2n_ensure.h"
#include "utils/s2n_result.h"
#include "utils/s2n_safety_macros.h"

#if defined(S2N_FAIL_THROUGH_SUPPORTED)
#define FALL_THROUGH __attribute__((fallthrough))
#else
#define FALL_THROUGH ((void) 0)
#endif

int s2n_in_unit_test_set(bool is_unit);
int s2n_in_integ_test_set(bool is_integ);
bool s2n_in_unit_test();
bool s2n_in_test();

bool s2n_constant_time_equals(const uint8_t* a, const uint8_t* b, const uint32_t len);
int s2n_constant_time_copy_or_dont(uint8_t* dst, const uint8_t* src, uint32_t len, uint8_t dont);

int s2n_constant_time_pkcs1_unpad_or_dont(uint8_t* dst, const uint8_t* src, uint32_t srclen, uint32_t expectlen);

#define DEFER_CLEANUP(_thealloc, _thecleanup) \
	__attribute__((cleanup(_thecleanup))) _thealloc

#define ZERO_TO_DISABLE_DEFER_CLEANUP(_thealloc) memset(&_thealloc, 0, sizeof(_thealloc))

S2N_CLEANUP_RESULT s2n_connection_apply_error_blinding(struct s2n_connection** conn);
#define WITH_ERROR_BLINDING(conn, action)	\
	do {	\
		DEFER_CLEANUP(struct s2n_connection* _conn_to_blind = conn, s2n_connection_apply_error_blinding);	\
		action;\
		if (_conn_to_blind) {	\
			_conn_to_blind = NULL;	\
		}	\
	} while (0)

#define DEFINE_POINTER_CLEANUP_FUNC(type, func)	\
	static inline void func##_pointer(type* p)	\
	{	\
		if (p && *p)	\
			func(*p);	\
	}	\
	struct __useless_struct_to_allow_trailing_semicolon__

#define s2n_array_len(array) (sizeof(array) / sizeof(array[0]))

int s2n_mul_overflow(uint32_t a, uint32_t b, uint32_t * out);

int s2n_align_to(uint32_t initial, uint32_t alignment, uint32_t * out);
int s2n_add_overflow(uint32_t a, uint32_t b, uint32_t * out);
int s2n_sub_overflow(uint32_t a, uint32_t b, uint32_t * out);
#define S2N_ADD_IS_OVERFLOW_SAFE(a, b, max) ((max) >= (a) && ((max) - (a) >= (b))) 
#pragma once

#include "error/s2n_errno.h"
#include "utils/s2n_ensure.h"
#include "utils/s2n_result.h"

#define _OSSL_SUCCESS 1
#define RESULT_BAIL(error) do { _S2N_ERROR((error)); __S2N_ENSURE_CHECKED_RETURN(S2N_RESULT_ERROR);} while (0)

#define RESULT_ENSURE(condition, error) __S2N_ENSURE((condition), RESULT_BAIL(error))
#define RESULT_DEBUG_ENSURE(condition, error) __S2N_ENSURE_DEBUG((condition), RESULT_BAIL(error))
#define RESULT_ENSURE_OK(result, error) __S2N_ENSURE(s2n_result_is_ok(result), RESULT_BAIL(error))
#define RESULT_ENSURE_GTE(a, b) __S2N_ENSURE((a) >= (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_LTE(a, b) __S2N_ENSURE((a) <= (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_GT(a, b) __S2N_ENSURE((a) > (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_LT(a, b) __S2N_ENSURE((a) < (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_EQ(a, b) __S2N_ENSURE((a) == (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_NE(a, b) __S2N_ENSURE((a) != (b), RESULT_BAIL(S2N_ERR_SAFETY))
#define RESULT_ENSURE_INCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(min) __tmp_min = (min);	\
		__typeof(max) __tmp_max = (max);	\
		RESULT_ENSURE_GTE(__tmp_n, __tmp_min);	\
		RESULT_ENSURE_LTE(__tmp_n, __tmp_max);	\
	} while (0)

#define RESULT_ENSURE_EXCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(min) __tmp_min = (min);	\
		__typeof(max) __tmp_max = (max);	\
		RESULT_ENSURE_GT(__tmp_n, __tmp_min);	\
		RESULT_ENSURE_LT(__tmp_n, __tmp_max);	\
	} while (0)

#define RESULT_ENSURE_REF(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_READABLE(x), RESULT_BAIL(S2N_ERR_NULL))
#define RESULT_ENSURE_MUT(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_WRITABLE(x), RESULT_BAIL(S2N_ERR_NULL))

#define RESULT_PRECONDITION(result) RESULT_GUARD(__S2N_ENSURE_PRECONDITION((result)))
#define RESULT_POSTCONDITION(result) RESULT_GUARD(__S2N_ENSURE_POSTCONDITION((result)))

#define RESULT_CHECKED_MEMCPY(destination, source, len) __S2N_ENSURE_SAFE_MEMMOVE((destination), (source), (len), RESULT_ENSURE_REF)
#define RESULT_CHECKED_MEMSET(destination, value, len) __S2N_ENSURE_SAFE_MEMSET((destination), (value), (len), RESULT_ENSURE_REF)

#define RESULT_GUARD(result) __S2N_ENSURE(s2n_result_is_ok(result), __S2N_ENSURE_CHECKED_RETURN(S2N_RESULT_ERROR))
#define RESULT_GUARD_OSSL(result, error) __S2N_ENSURE((result) == _OSSL_SUCCESS, RESULT_BAIL(error))
#define RESULT_GUARD_POSIX(result) __S2N_ENSURE((result) > S2N_FAILURE, __S2N_ENSURE_CHECKED_RETURN(S2N_RESULT_ERROR))
#define RESULT_GUARD_PTR(result) __S2N_ENSURE((result) != NULL, __S2N_ENSURE_CHECKED_RETURN(S2N_RESULT_ERROR))

#define POSIX_BAIL(error) do { _S2N_ERROR((error)); __S2N_ENSURE_CHECKED_RETURN(S2N_FAILURE);} while (0)

#define POSIX_ENSURE(condition, error) __S2N_ENSURE((condition), POSIX_BAIL(error))
#define POSIX_DEBUG_ENSURE(condition, error) __S2N_ENSURE_DEBUG((condition), POSIX_BAIL(error))

#define POSIX_ENSURE_OK(result, error) __S2N_ENSURE((result) > S2N_FAILURE, POSIX_BAIL(error))

#define POSIX_ENSURE_GTE(a, b) __S2N_ENSURE((a) >= (b), POSIX_BAIL(S2N_ERR_SAFETY))
#define POSIX_ENSURE_LTE(a, b) __S2N_ENSURE((a) <= (b), POSIX_BAIL(S2N_ERR_SAFETY))
#define POSIX_ENSURE_GT(a, b) __S2N_ENSURE((a) > (b), POSIX_BAIL(S2N_ERR_SAFETY))
#define POSIX_ENSURE_LT(a, b) __S2N_ENSURE((a) < (b), POSIX_BAIL(S2N_ERR_SAFETY))
#define POSIX_ENSURE_EQ(a, b) __S2N_ENSURE((a) == (b), POSIX_BAIL(S2N_ERR_SAFETY))
#define POSIX_ENSURE_NE(a, b) __S2N_ENSURE((a) != (b), POSIX_BAIL(S2N_ERR_SAFETY))

#define POSIX_ENSURE_INCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(n) __tmp_min = (min);	\
		__typeof(n) __tmp_max = (max);	\
		POSIX_ENSURE_GTE(__tmp_n, __tmp_min);	\
		POSIX_ENSURE_LTE(__tmp_n, __tmp_max);	\
	} while (0)

#define POSIX_ENSURE_EXCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(n) __tmp_min = (min);	\
		__typeof(n) __tmp_max = (max);	\
		POSIX_ENSURE_GT(__tmp_n, __tmp_min);	\
		POSIX_ENSURE_LT(__tmp_n, __tmp_max);	\
	} while (0)

#define POSIX_ENSURE_REF(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_READABLE(x), POSIX_BAIL(S2N_ERR_NULL))
#define POSIX_ENSURE_MUT(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_WRITABLE(x), POSIX_BAIL(S2N_ERR_NULL))
		
#define POSIX_PRECONDITION(result) POSIX_GUARD_RESULT(__S2N_ENSURE_PRECONDITION((result)))
#define POSIX_POSTCONDITION(result) POSIX_GUARD_RESULT(__S2N_ENSURE_POSTCONDITION((result)))
		
#define POSIX_CHECKED_MEMCPY(destination, source, len) __S2N_ENSURE_SAFE_MEMMOVE((destination), (source), (len), POSIX_ENSURE_REF)
#define POSIX_CHECKED_MEMSET(destination, value, len) __S2N_ENSURE_SAFE_MEMSET((destination), (value), (len), POSIX_ENSURE_REF)
		
#define POSIX_GUARD(result) __S2N_ENSURE((result) > S2N_FAILURE, __S2N_ENSURE_CHECKED_RETURN(S2N_FAILURE))
#define POSIX_GUARD_OSSL(result, error) __S2N_ENSURE((result) == _OSSL_SUCCESS, POSIX_BAIL(error))
#define POSIX_GUARD_RESULT(result) __S2N_ENSURE(s2n_result_is_ok(result), __S2N_ENSURE_CHECKED_RETURN(S2N_FAILURE))
#define POSIX_GUARD_PTR(result) __S2N_ENSURE((result) != NULL, __S2N_ENSURE_CHECKED_RETURN(S2N_FAILURE))

#define PTR_BAIL(error) do {_S2N_ERROR((error)); __S2N_ENSURE_CHECKED_RETURN(NULL);} while (0)
#define PTR_ENSURE(condition, error) __S2N_ENSURE((condition), PTR_BAIL(error))
#define PTR_DEBUG_ENSURE(condition, error) __S2N_ENSURE_DEBUG((condition), PTR_BAIL(error))

#define PTR_ENSURE_OK(result, error) __S2N_ENSURE((result) != NULL, PTR_BAIL(error))
#define PTR_ENSURE_GTE(a, b) __S2N_ENSURE((a) >= (b), PTR_BAIL(S2N_ERR_SAFETY))
#define PTR_ENSURE_LTE(a, b) __S2N_ENSURE((a) <= (b), PTR_BAIL(S2N_ERR_SAFETY))
#define PTR_ENSURE_GT(a, b) __S2N_ENSURE((a) > (b), PTR_BAIL(S2N_ERR_SAFETY))
#define PTR_ENSURE_LT(a, b) __S2N_ENSURE((a) < (b), PTR_BAIL(S2N_ERR_SAFETY))
#define PTR_ENSURE_EQ(a, b) __S2N_ENSURE((a) == (b), PTR_BAIL(S2N_ERR_SAFETY))
#define PTR_ENSURE_NE(a, b) __S2N_ENSURE((a) != (b), PTR_BAIL(S2N_ERR_SAFETY))

#define PTR_ENSURE_INCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(n) __tmp_min = (min);	\
		__typeof(n) __tmp_max = (max);	\
		PTR_ENSURE_GTE(__tmp_n, __tmp_min);	\
		PTR_ENSURE_LTE(__tmp_n, __tmp_max);	\
	} while (0)

#define PTR_ENSURE_EXCLUSIVE_RANGE(min, n, max)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		__typeof(n) __tmp_min = (min);	\
		__typeof(n) __tmp_max = (max);	\
		PTR_ENSURE_GT(__tmp_n, __tmp_min);	\
		PTR_ENSURE_LT(__tmp_n, __tmp_max);	\
	} while (0)

#define PTR_ENSURE_REF(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_READABLE(x), PTR_BAIL(S2N_ERR_NULL))
#define PTR_ENSURE_MUT(x) __S2N_ENSURE(S2N_OBJECT_PTR_IS_WRITABLE(x), PTR_BAIL(S2N_ERR_NULL))
#define PTR_PRECONDITION(result) PTR_GUARD_RESULT(__S2N_ENSURE_PRECONDITION((result)))
#define PTR_POSTCONDITION(result) PTR_GUARD_RESULT(__S2N_ENSURE_POSTCONDITION((result)))
#define PTR_CHECKED_MEMCPY(destination, source, len) __S2N_ENSURE_SAFE_MEMMOVE((destination), (source), (len), PTR_ENSURE_REF)
#define PTR_CHECKED_MEMSET(destination, value, len) __S2N_ENSURE_SAFE_MEMSET((destination), (value), (len), PTR_ENSURE_REF)

#define PTR_GUARD(result) __S2N_ENSURE((result) != NULL, __S2N_ENSURE_CHECKED_RETURN(NULL))
#define PTR_GUARD_OSSL(result, error) __S2N_ENSURE((result) == _OSSL_SUCCESS, PTR_BAIL(error))
#define PTR_GUARD_RESULT(result) __S2N_ENSURE(s2n_result_is_ok(result), __S2N_ENSURE_CHECKED_RETURN(NULL))
#define PTR_GUARD_POSIX(result) __S2N_ENSURE((result) > S2N_FAILURE, __S2N_ENSURE_CHECKED_RETURN(NULL))

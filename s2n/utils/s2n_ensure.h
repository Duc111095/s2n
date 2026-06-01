#pragma once

#define s2n_likely(x) __builtin_expect(!!(x), 1)
#define s2n_unlikely(x) __builtin_expect(!!(x), 0)

#define __S2N_ENSURE(cond, action)\
	do {	\
		if (!(cond)) { \
			action;	\
		}	\
	} while (0)

#define __S2N_ENSURE_LIKELY(cond, action)	\
	do {	\
		if (s2n_unlikely(!(cond))) {	\
			action;	\
		}	\
	} while (0)

#ifdef NDEBUG
	#define __S2N_ENSURE_DEBUG(cond, action)	\
	do {} while (0)

#else 
	#define __S2N_ENSURE_DEBUG(cond, action) __S2N_ENSURE_LIKELY((cond), action)
#endif

#define __S2N_ENSURE_PRECONDITION(result) (s2n_likely(s2n_result_is_ok(result)) ? S2N_RESULT_OK : S2N_RESULT_ERROR)

#ifdef NDEBUG
	#define __S2N_ENSURE_POSTCONDITION(result) (S2N_RESULT_OK)
#else
	#define __S2N_ENSURE_POSTCONDITION(result) (s2n_likely(s2n_result_is_ok(result)) ? S2N_RESULT_OK : S2N_RESULT_ERROR)
#endif

#define __S2N_ENSURE_SAFE_MEMMOVE(d, s, n, guard)	\
	do {	\
		__typeof(n) __tmp_n = (n);	\
		if (s2n_likely(__tmp_n)) {	\
			void *r = s2n_ensure_memmove_trace((d), (s), (__tmp_n));	\
			guard(r);	\
		}	\
	} while (0)

#define __S2N_ENSURE_SAFE_MEMSET(d, c, n, guard) \
	do {	\
		__typeof(n) __tmp_n = (n);	\
		if (s2n_likely(__tmp_n)) {	\
			__typeof(d) __tmp_d = (d);	\
			guard(__tmp_d);	\
			memset(__tmp_d, (c), __tmp_n);\
		}	\
	} while (0)

#if defined(S2N_DIAGNOSTICS_PUSH_SUPPORTED) && defined(S2N_DIAGNOSTICS_POP_SUPPORTED)
#define __S2N_ENSURE_CHECKED_RETURN(v)                                     \
        do {                                                                   \
            _Pragma("GCC diagnostic push")                                     \
                    _Pragma("GCC diagnostic error \"-Wconversion\"") return v; \
            _Pragma("GCC diagnostic pop")                                      \
        } while (0)
#else
#define __S2N_ENSURE_CHECKED_RETURN(v) return v
#endif

void* s2n_ensure_memmove_trace(void* to, const void* from, size_t size);

#ifdef CBMC
	#define S2N_MEM_IS_READABLE_CHECK(base, len) (((len) == 0) || __CPROVER_r_ok((base), (len)))
	#define S2N_MEM_IS_WRITABLE_CHECK(base, len) (((len) == 0) || __CPROVER_w_ok((base), (len)))
#else
#define S2N_MEM_IS_READABLE_CHECK(base, len) (((len) == 0) || (base) != NULL)
#define S2N_MEM_IS_WRITABLE_CHECK(base, len) (((len) == 0) || (base) != NULL)
#endif

#define S2N_MEM_IS_READABLE(base, len) (((len) == 0) || (base) != NULL)
#define S2N_MEM_IS_WRITABLE(base, len) (((len) == 0) || (base) != NULL)
#define S2N_OBJECT_PTR_IS_READABLE(ptr) ((ptr) != NULL)
#define S2N_OBJECT_PTR_IS_WRITABLE(ptr) ((ptr) != NULL)

#define S2N_IMPLIES(a, b) (!(a) || (b))

#define S2N_IFF(a, b) (!!(a) == !!(b))

#ifdef CBMC
#define CONTRACT_ASSERT(...)      __CPROVER_assert(__VA_ARGS__)
#define CONTRACT_ASSIGNS(...)     __CPROVER_assigns(__VA_ARGS__)
#define CONTRACT_ASSIGNS_ERR(...) CONTRACT_ASSIGNS(__VA_ARGS__, _s2n_debug_info, s2n_errno)
#define CONTRACT_ASSUME(...)      __CPROVER_assume(__VA_ARGS__)
#define CONTRACT_REQUIRES(...)    __CPROVER_requires(__VA_ARGS__)
#define CONTRACT_ENSURES(...)     __CPROVER_ensures(__VA_ARGS__)
#define CONTRACT_INVARIANT(...)   __CPROVER_loop_invariant(__VA_ARGS__)
#define CONTRACT_RETURN_VALUE     (__CPROVER_return_value)
#else
#define CONTRACT_ASSERT(...)
#define CONTRACT_ASSIGNS(...)
#define CONTRACT_ASSIGNS_ERR(...)
#define CONTRACT_ASSUME(...)
#define CONTRACT_REQUIRES(...)
#define CONTRACT_ENSURES(...)
#define CONTRACT_INVARIANT(...)
#define CONTRACT_RETURN_VALUE
#endif
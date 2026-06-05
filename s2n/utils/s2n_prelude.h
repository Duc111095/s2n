#pragma once

#define _S2N_PRELUDE_INCLUDED
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200809L
#endif

#if !defined(_FORTITY_SOURCE) && defined(S2N_BUILD_RELEASE)
#define _FORTITY_SOURCE 2
#endif

#if ((__GNUC__ >= 4) || defined(__clang__)) && defined(S2N_EXPORTS)
	#define S2N_API __attribute__((visibility("default")))
#endif

#define S2N_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define S2N_MAX(a, b) (((a) > (b)) ? (a) : (b))
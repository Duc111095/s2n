#pragma once

#include "utils/s2n_result.h"

#define S2N_IO_RETRY_EINTR(result, action)	\
	do {	\
		errno = 0;	\
		result = action;	\
	} while (result < 0 && errno == EINTR)

S2N_RESULT s2n_io_check_write_result(ssize_t result);
S2N_RESULT s2n_io_check_read_result(ssize_t result);
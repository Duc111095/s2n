#pragma once

#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

S2N_RESULT s2n_inet_ntop(int af, const void* addr, struct s2n_blob* dst);

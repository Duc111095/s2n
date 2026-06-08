#pragma once

#include "crypto/s2n_pkey.h"
#include "utils/s2n_result.h"

S2N_RESULT s2n_pkey_evp_init(struct s2n_pkey *pkey);

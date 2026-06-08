#pragma once

#include <stdbool.h>

#include "crypto/s2n_fips.h"
#include "utils/s2n_result.h"
#include "utils/s2n_safety.h"

bool s2n_pq_is_enabled(void);
bool s2n_libcrypto_supports_evp_kem(void);
bool s2n_libcrypto_supports_mlkem(void);



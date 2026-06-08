#pragma once

#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

S2N_RESULT s2n_prf_libcrypto(struct s2n_connection *conn,
    struct s2n_blob *secret, struct s2n_blob *label,
    struct s2n_blob *seed_a, struct s2n_blob *seed_b, struct s2n_blob *seed_c,
    struct s2n_blob *out);

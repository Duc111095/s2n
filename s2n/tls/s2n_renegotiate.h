#pragma once

#include "api/unstable/renegotiate.h"
#include "utils/s2n_result.h"

S2N_RESULT s2n_renegotiate_validate(struct s2n_connection *conn);

#pragma once

#include <openssl/evp.h>

#include "utils/s2n_safety.h"

DEFINE_POINTER_CLEANUP_FUNC(EVP_PKEY*, EVP_PKEY_free);
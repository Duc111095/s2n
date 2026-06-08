#pragma once

#include "utils/s2n_result.h"

bool s2n_libcrypto_is_openssl(void);
bool s2n_libcrypto_is_openssl_fips(void);
bool s2n_libcrypto_is_awslc(void);
bool s2n_libcrypto_is_awsls_fips(void);
bool s2n_libcrypto_is_boringssl(void);
bool s2n_libcrypto_is_libressl(void);

uint64_t s2n_libcrypto_awslc_api_version(void);
S2N_RESULT s2n_libcrypto_validate_runtime(void);
const char *s2n_libcrypto_get_version_name(void);
bool s2n_libcrypto_supports_flag_no_check_time(void);
bool s2n_libcrypto_supports_providers(void);
bool s2n_libcrypto_supports_custom_oid(void);


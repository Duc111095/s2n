#pragma once

#if defined(LIBRESSL_VERSION_NUMBER) && (OPENSSL_VERSION_NUMBER == 0x20000000L)
	#undef OPENSSL_VERSION_NUMBER
	#if LIBRESSL_VERSION_NUMBER < 0x3050000fL
		#define OPENSSL_VERSION_NUMBER 0x1000107fL
	#else
		#define OPENSSL_VERSION_NUMBER 0x1010000fL
	#endif
#endif

#define S2N_OPENSSL_VERSION_AT_LEAST(major, minor, fix)	\
	(OPENSSL_VERSION_NUMBER >= ((major << 28) + (minor << 20) + (fix << 12)))

#if (S2N_OPENSSL_VERSION_AT_LEAST(1, 1, 0)) && (!defined(OPENSSL_IS_BORINGSSL)) && (!defined(OPENSSL_IS_AWSLC)) && (!defined(LIBRESSL_VERSION_NUMBER))
#define s2n_evp_ctx_init(ctx) POSIX_GUARD_OSSL(EVP_CIPHER_CTX_init(ctx), S2N_ERR_RANDOM)
#define RESULT_EVP_CTX_INIT(ctx) RESULT_GUARD_OSSL(EVP_CIPHER_CTX_init(ctx), S2N_ERR_RANDOM)
#else
#define s2n_evp_ctx_init(ctx) EVP_CIPHER_CTX_init(ctx)
#define RESULT_EVP_CTX_INIT(ctx) EVP_CIPHER_CTX_init(ctx)
#endif

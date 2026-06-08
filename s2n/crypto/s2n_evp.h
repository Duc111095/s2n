#pragma once

#include <openssl/evp.h>
#include <openssl/hmac.h>

#include "crypto/s2n_openssl.h"
#include "utils/s2n_result.h"

struct s2n_evp_digest {
	const EVP_MD* md;
	EVP_MD_CTX* ctx;
};

#if S2N_OPENSSL_VERSION_AT_LEAST(1, 1, 0) && !defined(LIBRESSL_VERSION_NUMBER)
#define S2N_EVP_MD_CTX_NEW()         (EVP_MD_CTX_new())
#define S2N_EVP_MD_CTX_RESET(md_ctx) (EVP_MD_CTX_reset(md_ctx))
#define S2N_EVP_MD_CTX_FREE(md_ctx)  (EVP_MD_CTX_free(md_ctx))
#else
#define S2N_EVP_MD_CTX_NEW()         (EVP_MD_CTX_create())
#define S2N_EVP_MD_CTX_RESET(md_ctx) (EVP_MD_CTX_cleanup(md_ctx))
#define S2N_EVP_MD_CTX_FREE(md_ctx)  (EVP_MD_CTX_destroy(md_ctx))
#endif

#define S2N_EVP_PKEY_CTX_set_signature_md(ctx, md)  \
	EVP_PKEY_CTX_set_signature_md(ctx, (EVP_MD *) (uintptr_t) md)

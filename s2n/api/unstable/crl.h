#pragma once

#include <s2n.h>

struct s2n_crl_lookup;

typedef int (*s2n_crl_lookup_callback)(struct s2n_crl_lookup* lookup, void* context);

S2N_API int s2n_config_set_crl_lookup_cb(struct s2n_config* config, s2n_crl_lookup_callback callback, void* context);

S2N_API struct s2n_crl* s2n_crl_new(void);

S2N_API int s2n_crl_load_pem(struct s2n_crl* crl, uint8_t* pem, size_t len);

S2N_API int s2n_crl_free(struct s2n_crl** crl);

S2N_API int s2n_crl_get_issuer_hash(struct s2n_crl* crl, uint64_t* hash);

S2N_API int s2n_crl_validate_active(struct s2n_crl* crl);

S2N_API int s2n_crl_validate_not_expired(struct s2n_crl* crl);

S2N_API int s2n_crl_lookup_get_cert_issuer_hash(struct s2n_crl_lookup* lookup, uint64_t* hash);

S2N_API int s2n_crl_lookup_set(struct s2n_crl_lookup* lookup, struct s2n_crl* crl);

S2N_API int s2n_crl_lookup_ignore(struct s2n_crl_lookup* lookup);

struct s2n_cert_validation_info;

typedef int (*s2n_cert_validation_callback)(struct s2n_connection* conn, struct s2n_cert_validation_info* info,
	void* context);

S2N_API int s2n_config_set_cert_validation_cb(struct s2n_config* config, s2n_cert_validation_callback callback,
	void* context);

S2N_API int s2n_cert_validation_accept(struct s2n_cert_validation_info* info);

S2N_API int s2n_cert_validation_reject(struct s2n_cert_validation_info* info);
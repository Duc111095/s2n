#pragma once

#include "s2n.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#ifndef _WIN32
	#include <sys/uio.h>
#endif

struct s2n_certificate_request;
struct s2n_certificate_authority_list;

S2N_API struct s2n_certificate_authority_list* s2n_certificate_request_get_ca_list(struct s2n_certificate_request* request);

S2N_API int s2n_certificate_request_set_certificate(struct s2n_certificate_request* request, struct s2n_cert_chain_and_key* chain_and_key);

S2N_API bool s2n_certificate_authority_list_has_next(struct s2n_certificate_authority_list* list);

S2N_API int s2n_certificate_authority_list_next(struct s2n_certificate_authority_list* list, uint8_t** name, uint16_t* length);

S2N_API int s2n_certificate_authority_list_reread(struct s2n_certificate_authority_list* list);

typedef int (*s2n_cert_request_callback)(struct s2n_connection* conn, void* ctx, struct s2n_certificate_request* request);

S2N_API int s2n_config_set_cert_request_callback(struct s2n_config* config, s2n_cert_request_callback cert_req_callback, void* ctx);
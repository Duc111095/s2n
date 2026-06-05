#pragma once

#include <stdint.h>
#include "utils/s2n_blob.h"

int s2n_mem_init(void);
bool s2n_mem_is_init(void);
uint32_t s2n_mem_get_page_size(void);
int s2n_mem_cleanup(void);

int S2N_RESULT_MUST_USE s2n_alloc(struct s2n_blob* b, uint32_t size);
int S2N_RESULT_MUST_USE s2n_realloc(struct s2n_blob* b, uin32_t size);
int s2n_free(struct s2n_blob* b);
int s2n_free_without_wipe(struct s2n_blob* b);
int s2n_free_object(uin8_t** p_data, uint32_t size);
int S2N_RESULT_MUST_USE s2n_dup(struct s2n_blob* from, struct s2n_blob* to);

int s2n_free_or_wipe(struct s2n_blob* b);
S2N_RESULT s2n_mem_override_callbacks(s2n_mem_init_callback mem_init_cb, s2n_mem_cleanup_callback mem_cleanup_cb,
	s2n_mem_malloc_callback mem_malloc_cb, s2n_mem_free_callback mem_free_cb);
S2N_RESULT s2n_mem_get_callbacks(s2n_mem_init_callback* mem_init_cb, s2n_mem_cleanup_callback* mem_cleanup_cb,
	s2n_mem_malloc_callback *mem_malloc_cb, s2n_mem_free_callback *mem_free_cb);
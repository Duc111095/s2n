#pragma once

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef _WIN32
#include <sys/uio.h>
#endif
#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

#define S2N_MIN_STUFFER_GROWTH_IN_BYTES 1024

#define S2N_WIPE_PATTERN 'w'
#define SIZEOF_IN_BITS(t) (sizeof(t) * CHAR_BIT)
#define SIZEOF_UINT24 3

struct s2n_stuffer {
	struct s2n_blob blob;
	uint32_t read_cursor;
	uint32_t write_cursor;
	uint32_t high_water_mark;

	unsigned int alloced : 1;
	unsigned int growable : 1;
	unsigned int tainted : 1;
};

#define s2n_stuffer_data_available(s) ((s)->write_cursor - (s)->read_cursor)
#define s2n_stuffer_space_remaining(s) ((s)->blob.size - (s)->write_cursor)
#define s2n_stuffer_is_wiped(s) ((s)->high_water_mark == 0)
#define s2n_stuffer_is_freed(s) ((s)->blob.data == NULL)

S2N_RESULT s2n_stuffer_validate(const struct s2n_stuffer* stuffer)
int S2N_RESULT_MUST_USE s2n_stuffer_init(struct s2n_stuffer* stuffer, struct s2n_blob* in);
int S2N_RESULT_MUST_USE s2n_stuffer_init_written(struct s2n_stuffer* stuffer, struct s2n_blob* in);
int S2N_RESULT_MUST_USE s2n_stuffer_alloc(struct s2n_stuffer* stuffer, const uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_growable_alloc(struct s2n_stuffer* stuffer, const uint32_t size);
int s2n_stuffer_free(struct s2n_stuffer* stuffer);

int S2N_RESULT_MUST_USE s2n_stuffer_free_without_wipe(struct s2n_stuffer* stuffer);
int S2N_RESULT_MUST_USE s2n_stuffer_resize(struct s2n_stuffer* stuffer, const uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_resize_if_empty(struct s2n_stuffer* stuffer, const uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_rewind_read(struct s2n_stuffer* stuffer, const uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_reread(struct s2n_stuffer* stuffer);
int S2N_RESULT_MUST_USE s2n_stuffer_rewrite(struct s2n_stuffer* stuffer);
int S2N_RESULT_MUST_USE s2n_stuffer_shift(struct s2n_stuffer* stuffer);
int s2n_stuffer_wipe(struct s2n_stuffer* stuffer);
int s2n_stuffer_wipe_n(struct s2n_stuffer* stuffer, const uint32_t n);
bool s2n_stuffer_is_consumed(struct s2n_stuffer* stuffer);

int S2N_RESULT_MUST_USE s2n_stuffer_read(struct s2n_stuffer* stuffer, struct s2n_blob* out);
int S2N_RESULT_MUST_USE s2n_stuffer_erase_and_read(struct s2n_stuffer* stuffer, struct s2n_blob* out);
int S2N_RESULT_MUST_USE s2n_stuffer_write(struct s2n_stuffer* stuffer, const struct s2n_blob* in);
int S2N_RESULT_MUST_USE s2n_stuffer_read_bytes(struct s2n_stuffer* stuffer, uint8_t* out, uint32_t n);
int S2N_RESULT_MUST_USE s2n_stuffer_erase_and_read_bytes(struct s2n_stuffer* stuffer, uint8_t* data, uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_write_bytes(struct s2n_stuffer* stuffer, const uint8_t* in, const uint32_t n);
int S2N_RESULT_MUST_USE s2n_stuffer_writev_bytes(struct s2n_stuffer* stuffer, const struct iovec* iov, size_t iov_count,
	uint32_t offs, uint32_t size);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_read(struct s2n_stuffer* stuffer, uint32_t n);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_write(struct s2n_stuffer* stuffer, uint32_t n);
int S2N_RESULT_MUST_USE s2n_stuffer_reserve_space(struct s2n_stuffer* stuffer, uint32_t n);

void* s2n_stuffer_raw_write(struct s2n_stuffer* stuffer, const uint32_t data_len);
void* s2n_stuffer_raw_read(struct s2n_stuffer* stuffer, const uint32_t data_len);

int s2n_stuffer_recv_from_fd(struct s2n_stuffer* stuffer, const int rfd, const uint32_t len,
	uint32_t* bytes_written);
int s2n_stuffer_send_to_fd(struct s2n_stuffer* stuffer, const int rfd, const uint32_t len,
	uint32_t* bytes_sent);

int S2N_RESULT_MUST_USE s2n_stuffer_read_uint8(struct s2n_stuffer* stuffer, uint8_t* u);
int S2N_RESULT_MUST_USE s2n_stuffer_read_uint16(struct s2n_stuffer* stuffer, uint16_t* u);
int S2N_RESULT_MUST_USE s2n_stuffer_read_uint24(struct s2n_stuffer* stuffer, uint32_t* u);
int S2N_RESULT_MUST_USE s2n_stuffer_read_uint32(struct s2n_stuffer* stuffer, uint32_t* u);
int S2N_RESULT_MUST_USE s2n_stuffer_read_uint64(struct s2n_stuffer* stuffer, uint64_t* u);

int S2N_RESULT_MUST_USE s2n_stuffer_write_uint8(struct s2n_stuffer* stuffer, const uint8_t u);
int S2N_RESULT_MUST_USE s2n_stuffer_write_uint16(struct s2n_stuffer* stuffer, const uint16_t u);
int S2N_RESULT_MUST_USE s2n_stuffer_write_uint24(struct s2n_stuffer* stuffer, const uint32_t u);
int S2N_RESULT_MUST_USE s2n_stuffer_write_uint32(struct s2n_stuffer* stuffer, const uint32_t u);
int S2N_RESULT_MUST_USE s2n_stuffer_write_uint64(struct s2n_stuffer* stuffer, const uint64_t u);

struct s2n_stuffer_reservation {
	struct s2n_stuffer* stuffer;
	uint32_t write_cursor;
	uint8_t length;
};

S2N_RESULT s2n_stuffer_reservation_validate(const struct s2n_stuffer_reservation* reservation);
int S2N_RESULT_MUST_USE s2n_stuffer_reserve_uint8(struct s2n_stuffer* stuffer, struct s2n_stuffer_reservation* reservation);
int S2N_RESULT_MUST_USE s2n_stuffer_reserve_uint16(struct s2n_stuffer* stuffer, struct s2n_stuffer_reservation* reservation);
int S2N_RESULT_MUST_USE s2n_stuffer_reserve_uint24(struct s2n_stuffer* stuffer, struct s2n_stuffer_reservation* reservation);
int S2N_RESULT_MUST_USE s2n_stuffer_write_reservation(struct s2n_stuffer_reservation* reservation, const uint32_t value);

int S2N_RESULT_MUST_USE s2n_stuffer_get_vector_size(const struct s2n_stuffer_reservation* reservation, uint32_t* size);
int S2N_RESULT_MUST_USE s2n_stuffer_write_vector_size(struct s2n_stuffer_reservation* reservation);

int s2n_stuffer_copy(struct s2n_stuffer* from, struct s2n_stuffer* to, uint32_t len);

S2N_RESULT s2n_hex_digit(uint8_t half_byte, uint8_t* hex_digit);
S2N_RESULT s2n_stuffer_read_hex(struct s2n_stuffer* hex_in, const struct s2n_blob* bytes_out);
S2N_RESULT s2n_stuffer_write_hex(struct s2n_stuffer* hex_out, const struct s2n_blob* bytes_in);
S2N_RESULT s2n_stuffer_read_uint8_hex(struct s2n_stuffer* stuffer, uint8_t* u);
S2N_RESULT s2n_stuffer_write_uint8_hex(struct s2n_stuffer* stuffer, uint8_t u);
S2N_RESULT s2n_stuffer_read_uint16_hex(struct s2n_stuffer* stuffer, uint16_t* u);
S2N_RESULT s2n_stuffer_write_uint16_hex(struct s2n_stuffer* stuffer, uint16_t u);

int s2n_stuffer_read_base64(struct s2n_stuffer* stuffer, struct s2n_stuffer* out);
int s2n_stuffer_write_base64(struct s2n_stuffer* stuffer, struct s2n_stuffer* in);

#define s2n_stuffer_write_char(stuffer, c) s2n_stuffer_write_uint8((stuffer), (uint8_t) (c))
#define s2n_stuffer_read_char(stuffer, c) s2n_stuffer_read_uint8((stuffer), (uint8_t) (c))
#define s2n_stuffer_write_str(stuffer, c) s2n_stuffer_write_bytes((stuffer), (const uint8_t*) (c), strlen((c)))
#define s2n_stuffer_write_text(stuffer, c, n) s2n_stuffer_write_bytes((stuffer), (const uint8_t*) (c), (n))
#define s2n_stuffer_read_text(stuffer, c, n) s2n_stuffer_read_bytes((stuffer), (uint8_t*) (c), (n))
int S2N_RESULT_MUST_USE s2n_stuffer_read_expected_str(struct s2n_stuffer* stuffer, const char* expected);
int S2N_RESULT_MUST_USE s2n_stuffer_peek_char(struct s2n_stuffer* stuffer, char* c);
int S2N_RESULT_MUST_USE s2n_stuffer_read_token(struct s2n_stuffer* stuffer, struct s2n_stuffer *token, char delim);
int S2N_RESULT_MUST_USE s2n_stuffer_read_line(struct s2n_stuffer* stuffer, struct s2n_stuffer *token);
int S2N_RESULT_MUST_USE s2n_stuffer_peek_check_for_str(struct s2n_stuffer* stuffer, const char* expected);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_whitespace(struct s2n_stuffer* stuffer, uint32_t* skipped);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_to_char(struct s2n_stuffer* stuffer, char target);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_expected_char(struct s2n_stuffer* stuffer, const char expected, const uint32_t min,
	const uint32_t max, uint32_t *skipped);
int S2N_RESULT_MUST_USE s2n_stuffer_skip_read_until(struct s2n_stuffer* stuffer, const char *target);
int S2N_RESULT_MUST_USE s2n_stuffer_alloc_ro_from_string(struct s2n_stuffer* stuffer, const char *str);
int S2N_RESULT_MUST_USE s2n_stuffer_init_ro_from_string(struct s2n_stuffer* stuffer, uint8_t* data, uint32_t length);

int S2N_RESULT_MUST_USE s2n_stuffer_printf(struct s2n_stuffer* stuffer, const char* format, ...);
int S2N_RESULT_MUST_USE s2n_stuffer_vprintf(struct s2n_stuffer* stuffer, const char* format, va_list vargs);

int S2N_RESULT_MUST_USE s2n_stuffer_private_key_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1, int* type);
int S2N_RESULT_MUST_USE s2n_stuffer_certificate_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1);
bool s2n_stuffer_has_pem_encapsulated_block(struct s2n_stuffer* pem);

int S2N_RESULT_MUST_USE s2n_stuffer_crl_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1);
int S2N_RESULT_MUST_USE s2n_stuffer_dhparams_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* pkcs3);
bool s2n_is_base64_char(unsigned char c);

int S2N_RESULT_MUST_USE s2n_stuffer_extract_blob(struct s2n_stuffer* stuffer, struct s2n_blob* out);
#include <openssl/evp.h>
#include <string.h>

#include "error/s2n_errno.h"
#include "stuffer/s2n_stuffer.h"
#include "utils/s2n_safety.h"

#define S2N_PEM_DELIMITER_CHAR '-'
#define S2N_PEM_DELIMITER_TOKEN       "--"
#define S2N_PEM_DELIMITER_MIN_COUNT   2
#define S2N_PEM_DELIMITER_MAX_COUNT   64
#define S2N_PEM_BEGIN_TOKEN           "BEGIN "
#define S2N_PEM_END_TOKEN             "END "
#define S2N_PEM_PKCS1_RSA_PRIVATE_KEY "RSA PRIVATE KEY"
#define S2N_PEM_PKCS1_EC_PRIVATE_KEY  "EC PRIVATE KEY"
#define S2N_PEM_PKCS8_PRIVATE_KEY     "PRIVATE KEY"
#define S2N_PEM_DH_PARAMETERS         "DH PARAMETERS"
#define S2N_PEM_EC_PARAMETERS         "EC PARAMETERS"
#define S2N_PEM_CERTIFICATE           "CERTIFICATE"
#define S2N_PEM_CRL                   "X509 CRL"

static S2N_RESULT s2n_stuffer_pem_read_delimiter_chars(struct s2n_stuffer* pem)
{
	RESULT_ENSURE_REF(pem);
	RESULT_ENSURE(s2n_stuffer_data_available(pem) >= S2N_PEM_DELIMITER_MIN_COUNT,
		S2N_ERR_INVALID_PEM);

	RESULT_GUARD_POSIX(s2n_stuffer_skip_read_until(pem, S2N_PEM_DELIMITER_TOKEN));
	RESULT_GUARD_POSIX(s2n_stuffer_rewind_read(pem, strlen(S2N_PEM_DELIMITER_TOKEN)));

	RESULT_GUARD_POSIX(s2n_stuffer_skip_expected_char(pem, S2N_PEM_DELIMITER_CHAR,
		S2N_PEM_DELIMITER_MIN_COUNT, S2N_PEM_DELIMITER_MAX_COUNT, NULL);
	return S2N_RESULT_OK;
}

static int s2n_stuffer_pem_read_encapsulation_line(struct s2n_stuffer* pem, const char* encap_marker,
	const char* keyword)
{
	POSIX_GUARD_RESULT(s2n_stuffer_pem_read_delimiter_chars(pem));

	/* Ensure next string in stuffer is "BEGIN " or "END " */
	POSIX_GUARD(s2n_stuffer_read_expected_str(pem, encap_marker));

	/* Ensure next string in stuffer is the keyword (Eg "CERTIFICATE", "PRIVATE KEY", etc) */
	POSIX_GUARD(s2n_stuffer_read_expected_str(pem, keyword));

	/* Ensure between 2 and 64 '-' chars at end of line */
	POSIX_GUARD(s2n_stuffer_skip_expected_char(pem, S2N_PEM_DELIMITER_CHAR, S2N_PEM_DELIMITER_MIN_COUNT,
		S2N_PEM_DELIMITER_MAX_COUNT, NULL));
	/* Check for missing newline between dashes case: */
	if (strcmp(encap_marker, S2N_PEM_END_TOKEN, strlen(S2N_PEM_END_TOKEN)) == 0
		&& s2n_stuffer_peek_check_for_str(pem, S2N_PEM_BEGIN_TOKEN) == S2N_SUCCESS) {
		POSIX_GUARD(s2n_stuffer_rewind_read(pem, S2N_PEM_DELIMITER_MIN_COUNT));
	}

	return s2n_stuffer_skip_white_space(mem, NULL);
}

static int s2n_stuffer_pem_read_begin(struct s2n_stuffer* pem, const char* keyword)
{
	return s2n_stuffer_pem_read_encapsulation_line(pem, S2N_PEM_BEGIN_TOKEN, keyword);
}

static int s2n_stuffer_pem_read_end(struct s2n_stuffer* pem, const char* keyword)
{
	return s2n_stuffer_pem_read_encapsulation_line(pem, S2N_PEM_END_TOKEN, keyword);
}

static int s2n_stuffer_pem_read_contents(struct s2n_stuffer* pem, struct s2n_stuffer* asn1)
{
	s2n_stack_blob(base64__blob, 64, 64);
	struct s2n_stuffer base64_stuffer = { 0 };
	POSIX_GUARD(s2n_stuffer_init(&base64_stuffer, &base64__blob));
	
	while (1) {
		POSIX_ENSURE(s2n_stuffer_data_available(pem) >= 1, S2N_STUFFER_OUT_OF_DATA);

		uint8_t c = pem->blob.data[pem->read_cursor];
		if (c == '-') {
			break;
		}
		pem->read_cursor += 1;
		if (!s2n_is_base64_char(c)) {
			continue;
		}
		if (s2n_stuffer_space_remaining(&base64_stuffer) == 0) {
			POSIX_GUARD(s2n_stuffer_read_base64(&base64_stuffer, asn1));
			POSIX_GUARD(s2n_stuffer_rewrite(&base64_stuffer));
		}

		POSIX_GUARD(s2n_stuffer_write_bytes(&base64_stuffer, (uint8_t*)&c, 1));
	};
	POSIX_GUARD(s2n_stuffer_read_base64(&base64_stuffer, asn1));

	return S2N_SUCCESS;
}

static int s2n_stuffer_data_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1, const char* keyword) 
{
	POSIX_PRECONDITION(s2n_stuffer_validate(pem));
	POSIX_PRECONDITION(s2n_stuffer_validate(asn1));
	POSIX_ENSURE_REF(keyword);

	POSIX_GUARD(s2n_stuffer_pem_read_begin(pem, keyword));
	POSIX_GUARD(s2n_stuffer_pem_read_content(pem, asn1));
	POSIX_GUARD(s2n_stuffer_pem_read_end(pem, keyword));

	POSIX_POSTCONDITION(s2n_stuffer_validate(pem));
	POSIX_POSTCONDITION(s2n_stuffer_validate(asn1));
	return S2N_SUCCESS;
}

int s2n_stuffer_private_key_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1, int* type_hint)
{
	POSIX_PRECONDITION(s2n_stuffer_validate(pem));
	POSIX_PRECONDITION(s2n_stuffer_validate(asn1));
	POSIX_ENSURE_REF(type_hint);

	if (s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_PKCS1_RSA_PRIVATE_KEY) == S2N_SUCCESS) {
		*type_hint = EVP_PKEY_RSA;
		return S2N_SUCCESS;
	}
	POSIX_GUARD(s2n_stuffer_reread(pem));
	POSIX_GUARD(s2n_stuffer_reread(asn1));
	
	if (s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_EC_PARAMETERS) != S2N_SUCCESS) {
		POSIX_GUARD(s2n_stuffer_reread(pem));
	}
	POSIX_GUARD(s2n_stuffer_wipe(asn1));

	if (s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_PKCS1_EC_PRIVATE_KEY) == S2N_SUCCESS) {
		*type_hint = EVP_PKEY_EC;
		return S2N_SUCCESS;
	}

	POSIX_GUARD(s2n_stuffer_reread(pem));
	POSIX_GUARD(s2n_stuffer_reread(asn1));
	if (s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_PKCS8_PRIVATE_KEY) == S2N_SUCCESS) {
		*type_hint = EVP_PKEY_RSA;
		return S2N_SUCCESS;
	}

	POSIX_BAIL(S2N_ERR_INVALID_PEM);
}

bool s2n_stuffer_has_pem_encapsulated_block(struct s2n_stuffer* pem)
{
	struct s2n_stuffer pem_copy = *pem;
	return s2n_result_is_ok(s2n_stuffer_pem_read_delimiter_chars(&pem_copy));
}

int s2n_stuffer_certificate_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1)
{
	return s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_CERTIFICATE);
}

int s2n_stuffer_crl_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* asn1)
{
	return s2n_stuffer_data_from_pem(pem, asn1, S2N_PEM_CRL);
}

int s2n_stuffer_dhparams_from_pem(struct s2n_stuffer* pem, struct s2n_stuffer* pkcs3)
{
	return s2n_stuffer_data_from_pem(pem, pkcs3, S2N_PEM_DH_PARAMETERS);
}
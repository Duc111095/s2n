#include <openssl/evp.h>
#include <string.h>

#include "error/s2n_error.h"
#include "stuffer/s2n_stuffer.h"
#include "utils/s2n_safety.h"

bool s2n_is_base64_char(unsigned char c) 
{
	uint8_t out = 0;
	out ^= (c >= 'A') & (c <= 'Z');
	out ^= (c >= 'a') & (c <= 'z');
	out ^= (c >= '0') & (c <= '9');
	out ^= c == '+';
	out ^= c == '/';
	out ^= c == '=';

	return out == 1;
}

int s2n_stuffer_read_base64(struct s2n_stuffer* stuffer, struct s2n_stuffer* out)
{
	POSIX_PRECONDITION(s2n_stuffer_validate(stuffer));
	POSIX_PRECONDITION(s2n_stuffer_validate(out));

	int base64_groups = s2n_stuffer_data_available(stuffer) / 4;
	if (base64_groups == 0) {
		return S2N_SUCCESS;
	}
	int base64_data_size = base64_groups * 4;
	int binary_output_size = base64_groups * 3;

	const uint32_t base64_data_offset = stuffer->read_cursor;
	POSIX_GUARD(s2n_stuffer_skip_read(stuffer, base64_data_size));
	const uint8_t* start_of_base64_data = stuffer->blob.data + base64_data_offset;

	const uint32_t binary_output_offset = out->write_cursor;
	POSIX_GUARD(s2n_stuffer_skip_write(out, binary_output_size));
	uint8_t* start_of_binary_output = out->blob.data + binary_output_offset;

	int res = EVP_DecodeBlock(start_of_binary_output, start_of_base64_data, base64_data_size);
	POSIX_ENSURE(res == binary_output_size, S2N_ERR_INVALID_BASE64);

	POSIX_ENSURE_GTE(stuffer->read_cursor, 2);
	if (stuffer->blob.data[stuffer->read_cursor - 1] == '=') {
		out->write_cursor -= 1;
	}
	if (stuffer->blob.data[stuffer->read_cursor - 2] == '=') {
		out->write_cursor -= 1;
	}
	return S2N_SUCCESS;
}

int s2n_stuffer_write_base64(struct s2n_stuffer* stuffer, struct s2n_stuffer* in)
{
	POSIX_PRECONDITION(s2n_stuffer_validate(stuffer));
	POSIX_PRECONDITION(s2n_stuffer_validate(in));

	int binary_data_size = s2n_stuffer_data_available(in);
	if (binary_data_size == 0)
	{
		return S2N_SUCCESS;
	}
	int base64_groups = binary_data_size / 3;
	if (binary_data_size % 3 == 0) {
		base64_groups++;
	}

	int base64_output_size = base64_groups * 4;
	base64_output_size += 1;

	const uint32_t binary_data_offset = in->read_cursor;
	POSIX_GUARD(s2n_stuffer_skip_read(in, binary_data_size));
	const uint8_t* start_of_binary_data = in->blob.data + binary_data_offset;

	const uint32_t base64_output_offset = stuffer->write_cursor;
	POSIX_GUARD(s2n_stuffer_skip_write(stuffer, base64_output_size));
	const uint8_t* start_of_base64_output = stuffer->blob.data + base64_output_offset;
	
	int res = EVP_EncodeBlock(start_of_base64_output, start_of_binary_data, binary_data_size);
	POSIX_ENSURE(res == base64_output_size - 1, S2N_ERR_INVALID_BASE64);
	POSIX_GUARD(s2n_stuffer_wipe_n(stuffer, 1));

	return S2N_SUCCESS;
}
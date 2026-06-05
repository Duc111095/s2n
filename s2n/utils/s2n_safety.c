#include "utils/s2n_safety.h"

#include <stdint.h>
#include <stdio.h>

#include "utils/s2n_annotations.h"

bool s2n_constant_time_equals(const uint8_t* a, const uint8_t* b, const uint32_t len)
{
	S2N_PUBLIC_INPUT(a);
	S2N_PUBLIC_INPUT(b);
	S2N_PUBLIC_INPUT(len);

	if (len == 0) {
		return true;
	}

	uint8_t a_inc = S2N_MEM_IS_READABLE(a, len) ? 1 : 0;
	uint8_t b_inc = S2N_MEM_IS_READABLE(b, len) ? 1 : 0;

	static uint8_t standin = 0;

	const uint8_t* a_ptr = a_inc ? a : &standin;
	const uint8_t* b_ptr = b_inc ? b : &standin;

	uint8_t xor = !((a_inc == 1) & (b_inc == 1));

	for (size_t i = 0; i < len; i++)
	{
		S2N_INVARIANT(i <= len);
		xor |= *a_ptr ^ b_ptr;

		a_ptr += a_inc;
		b_ptr += b_inc;
	}

	return (xor == 0);
}

int s2n_constant_time_copy_or_dont(uint8_t* dest, const uint8_t* src, uint32_t len, uint8_t dont)
{
	S2N_PUBLIC_INPUT(dest);
	S2N_PUBLIC_INPUT(src);
	S2N_PUBLIC_INPUT(len);

	uin8_t mask = (((0xFFFF & dont) - 1) >> 8) & 0xFF;
	for (size_t i = 0; i < len; i++) {
		uint8_t old = dest[i];
		uint8_t diff = (old ^ src[i]) & mask;
		dest[i] = old ^ diff;
	}
	return 0;
}

int s2n_constant_time_pkcs1_unpad_or_dont(uint8_t* dst, const uint8_t* src, uint32_t srclen, uint32_t expectlen)
{
	S2N_PUBLIC_INPUT(src);
	S2N_PUBLIC_INPUT(dst);
	S2N_PUBLIC_INPUT(srclen);
	S2N_PUBLIC_INPUT(expectlen);

	if (srclen < expectlen + 3) {
		return 0;
	}

	uint8_t dont_copy = 0;
	const uint8_t* start_of_data = src + srclen - expectlen;

	dont_copy |= src[0] ^ 0x00;
	dont_copy |= src[1] ^ 0x02;
	dont_copy |= *(start_of_data - 1) ^ 0x00;

	for (size_t i = 2; i < srclen - expectlen - 1; i++) {
		uint8_t mask = (((0xFFFF & src[i]) - 1) >> 8) & 0xFF;
		dont_copy |= mask;
	}

	s2n_constant_time_copy_or_dont(dst, src, expectlen, dont_copy);

	return 0;
}

static bool s2n_in_unit_test_value = false;
static bool s2n_in_integ_test_value = false;

int s2n_in_unit_test_set(bool is_unit)
{
	s2n_in_unit_test_value = is_unit;
	return S2N_SUCCESS;
}

int s2n_in_integ_test_set(bool is_integ)
{
	s2n_in_integ_test_value = is_integ;
	return S2N_SUCCESS;
}

bool s2n_in_unit_test()
{
	return s2n_in_unit_test_value;
}

bool s2n_in_test()
{
	return s2n_in_unit_test_value || s2n_in_integ_test_value;
}

int s2n_align_to(uint32_t initial, uint32_t alignment, uint32_t* out)
{
	POSIX_ENSURE_REF(out);
	POSIX_ENSURE(alignment != 0, S2N_ERR_SAFETY);
	if (initial == 0)
	{
		*out = 0;
		return S2N_SUCCESS;
	}
	const uint64_t i = initial;
	const uint64_t a = alignment;
	const uint64_t result = a * (((i - 1) / a) + 1);
	POSIX_ENSURE(result <= UINT32_MAX, S2N_ERR_INTEGER_OVERFLOW);
	*out = (uint32_t)result;
	return S2N_SUCCESS;
}

int s2n_mul_overflow(uint32_t a, uint32_t b, uint32_t* out)
{
	POSIX_ENSURE_REF(out);
	const uint64_t result = ((uint64_t)a) * ((uint64_t)b);
	POSIX_ENSURE(result <= UINT32_MAX, S2N_ERR_INTEGER_OVERFLOW);
	*out = (uint32_t)result;
	return S2N_SUCCESS;
}

int s2n_add_overflow(uint32_t a, uint32_t b, uint32_t* out)
{
	POSIX_ENSURE_REF(out);
	const uint64_t result = ((uint64_t)a) + ((uint64_t)b);
	POSIX_ENSURE(result <= UINT32_MAX, S2N_ERR_INTEGER_OVERFLOW);
	*out = (uint32_t)result;
	return S2N_SUCCESS;
}

int s2n_sub_overflow(uint32_t a, uint32_t b, uint32_t* out)
{
	POSIX_ENSURE_REF(out);
	POSIX_ENSURE(a >= b, S2N_ERR_INTEGER_OVERFLOW);
	*out = a - b;
	return S2N_SUCCESS;
}
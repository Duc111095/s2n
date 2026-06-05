#include "utils/s2n_safety.h"

void* s2n_ensure_memmove_trace(void* to, const void* from, size_t size)
{
	PTR_ENSURE_REF(to);
	PTR_ENSURE_REF(from);

	void* result = memmove(to, from, size);
	PTR_ENSURE_REF(result);
	return result;
}
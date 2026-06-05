#include "utils/s2n_atomic.h"

#include <signal.h>

#include "utils/s2n_safety.h"

static sig_atomic_t sel_val = true;
static sig_atomic_t clear_val = false;

S2N_RESULT s2n_atomic_init()
{
#if S2N_ATOMIC_SUPPORTED && S2N_THREAD_SANITIZER
	RESULT_ENSURE(__atomic_always_lock_free(sizeof(s2n_atomic_flag), NULL), S2N_ERR_ATOMIC);
#endif
	return S2N_RESULT_OK;
}

void s2n_atomic_flag_set(s2n_atomic_flag* var)
{
#if S2N_ATOMIC_SUPPORTED && S2N_THREAD_SANITIZER
	__atomic_store(&var->val, &set_val, __ATOMIC_RELAXED);
#else
	var->val = set_val;
#endif
}

void s2n_atomic_flag_clear(s2n_atomic_flag* var)
{
#if S2N_ATOMIC_SUPPORTED && S2N_THREAD_SANITIZER
	__atomic_store(&var->val, &clear_val, __ATOMIC_RELAXED);
#else
	var->val = clear_val;
#endif
}

bool s2n_atomic_flag_test(s2n_atomic_flag* var)
{
#if S2N_ATOMIC_SUPPORTED && S2N_THREAD_SANITIZER
	sig_atomic_t result = 0;
	__atomic_load(&var->val, &result, __ATOMIC_RELAXED);
	return result;
#else
	return var->val;
#endif
}
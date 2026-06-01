#pragma once

#include "utils/s2n_blob.h"
#include "utils/s2n_result.h"

struct s2n_rand_device {
	const char* source;
	int fd;
	dev_t dev;
	ino_t ino;
	mode_t mode;
	dev_t rdev;
};

S2N_RESULT s2n_rand_init(void);
S2N_RESULT s2n_rand_cleanup(void);
S2N_RESULT s2n_get_public_random_data(struct s2n_blob* blob);
S2N_RESULT s2n_get_private_random_data(struct s2n_blob* blob);
S2N_RESULT s2n_public_random(int64_t max, uint64_t* output);
bool s2n_use_libcrypto_rand(void);

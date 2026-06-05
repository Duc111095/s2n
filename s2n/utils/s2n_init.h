#pragma once

#include <stdbool.h>

int s2n_init(void);
int s2n_cleanup(void);
bool s2n_is_initialized(void);
int s2n_enable_atexit(void);
#pragma once

#define S2N_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#define S2N_GCC_VERSION_AT_LEAST(major, minor, patch_level) \
	((S2N_GCC_VERSION) >= ((major) * 10000 + (minor) * 100 + (patch_level)))
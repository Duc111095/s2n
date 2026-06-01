#pragma once
#include <s2n.h>

S2N_API extern int s2n_config_add_custom_x509_extension(struct s2n_config* config, uint8_t* extension_oid, uint32_t extension_oid_len);
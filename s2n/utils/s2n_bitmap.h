#pragma once

#define S2N_CBIT_BIT(bit) (1 << ((bit) % 8))
#define S2N_CBIT_BIN(mask, bit) (mask)[(bit) >> 3]
#define S2N_CBIT_SET(mask, bit) ((void) (S2N_CBIT_BIN(mask, bit) |= S2N_CBIT_BIT(bit)))
#define S2N_CBIT_CLR(mask, bit) ((void) (S2N_CBIT_BIN(mask, bit) &= ~S2N_CBIT_BIT(bit)))
#define S2N_CBIT_TEST(mask, bit) ((S2N_CBIT_BIN(mask, bit) & S2N_CBIT_BIT(bit)) != 0)
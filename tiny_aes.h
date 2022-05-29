#ifndef TINY_AES_H
#define TINY_AES_H

#include <stdint.h>

#define USE_LOOKUP_TABLE 1

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];

#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1
typedef uint8_t v16si __attribute__ ((vector_size (16)));

v16si AddRoundKey(v16si state, const v16si RoundKey);

#else

void AddRoundKey(state_t* state, const uint8_t* RoundKey);

#endif // GCC_VECTOR_EXTENSIONS

void SubBytes(state_t* state);
void ShiftRows(state_t* state);
void MixColumns(state_t* state);

#endif
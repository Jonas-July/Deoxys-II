#ifndef TINY_AES_H
#define TINY_AES_H

#include <stdint.h>

#define USE_LOOKUP_TABLE 1

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];

void AddRoundKey(state_t* state, const uint8_t* RoundKey);
void SubBytes(state_t* state);
void ShiftRows(state_t* state);
void MixColumns(state_t* state);

#endif
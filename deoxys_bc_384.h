#ifndef DEOXYS_BC_384_H
#define DEOXYS_BC_384_H

#include <stdint.h>

// Set to 1 to use vector extensions
#define GCC_VECTOR_EXTENSIONS 0

/**
	Version for 256-bit key + 128 bit tweak
*/
uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext);


#endif
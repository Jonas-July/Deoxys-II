#ifndef DEOXYS_BC_384_H
#define DEOXYS_BC_384_H

#include <stdint.h>

/**
	Version for 256-bit key + 128 bit tweak
*/
void Deoxys_BC_encrypt_buffer(uint8_t* buffer, uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext);
uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext);


#endif
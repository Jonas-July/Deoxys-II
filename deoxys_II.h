#ifndef DEOXYS_II_H
#define DEOXYS_II_H

#include <stdint.h>

uint8_t* Deoxys_II_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size);

/*
	Decrypts the given ciphertext and authenticates the additional data.

	If authentication failed, then authentication_failed is 1 and the return value is empty.
	If authentication was successful, then authentication_failed is 0,
	the decrypted message is returned and its size written to buffer_message_size.

*/
uint8_t* Deoxys_II_decrypt_buffer(	int* authentication_failed, uint32_t* buffer_message_size,
					uint8_t const* key, uint8_t const* nonce, uint8_t const* tag,
					uint8_t const* buffer_ciphertext, uint32_t buffer_ciphertext_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size);



#endif

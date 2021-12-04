#ifndef DEOXYS_II_H
#define DEOXYS_II_H

#include <stdint.h>

uint8_t* Deoxys_II_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size, 
					uint8_t const* buffer_ad, uint32_t buffer_ad_size);

void Deoxys_II_decrypt_buffer(	uint8_t const* key, uint8_t const* nonce, uint8_t const* tag, 
				uint8_t const* buffer_ciphertext, uint32_t buffer_ciphertext_size, 
				uint8_t const* buffer_ad, uint32_t buffer_ad_size);



#endif

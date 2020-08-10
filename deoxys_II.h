#ifndef DEOXYS_II_H
#define DEOXYS_II_H


#include <stdint.h>


#define BLOCK_LEN	16 //Block length in Deoxys is 128 bit only
#define TAG_LEN		BLOCK_LEN //Tag length in Deoxys is 128 bit only
#define TWEAK_LEN	16 //Tweak length in Deoxys is 128 bit
#define NONCE_LEN	15 //Nonce length in Deoxys-II is 120 bit


uint8_t* Deoxys_II_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size, 
					uint8_t const* buffer_ad, uint32_t buffer_ad_size);

void Deoxys_II_decrypt_buffer(	uint8_t const* key, uint8_t const* nonce, uint8_t const* tag, 
				uint8_t const* buffer_ciphertext, uint32_t buffer_ciphertext_size, 
				uint8_t const* buffer_ad, uint32_t buffer_ad_size);

uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* message);



#endif

/*
	Copyright Jonas July 2022.
	Licensed under the EUPL-1.2 or later.

	Implementation of the Deoxys-AE2 AEAD Operating Mode as described in
	https://github.com/deoxys-crypto/deoxys-tbc/blob/master/draft-irtf-cfrg-deoxys-tbc.md

	Last modified: 2022-05-29 by Jonas July
*/
#include <stdlib.h>
#include <string.h>

#include "deoxys_AE2.h"
#include "deoxys_bc_384.h"

#define KEY_LEN		16 // Key length is 128 bit in this mode
#define BLOCK_LEN	16 //Block length in Deoxys is 128 bit only
#define DBL_BLOCK_LEN	32
#define TAG_LEN		BLOCK_LEN //Tag length in Deoxys is 128 bit only
#define TWEAK_LEN	16 //Tweak length in Deoxys is 128 bit
#define NONCE_LEN	16 //Nonce length in Deoxys-AE2 is 128 bit

#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1

/**
	Computes the xor of first and second bytewise
	both input arrays must be of size BLOCK_LEN
	first argument is overwritten with the result
*/
void XOR_block_override(uint8_t* first, uint8_t const* second, int length)
{
	v16si* f = first;
	v16si* s = second;
	*f = *f ^ *s;
}

#else

/**
	Computes the xor of first and second bytewise
	both input arrays must be of size BLOCK_LEN
	first argument is overwritten with the result
*/
void XOR_block_override(uint8_t* first, uint8_t const* second, int length)
{
        for (int i = 0; i < length; i++) {
		first[i] = first[i] ^ second[i];
	}
}

#endif // GCC_VECTOR_EXTENSIONS

/*
	Processes 2 128-bit blocks at the same time
*/
void compute_partial_tag(
		uint8_t* tag,
		uint8_t const* key,
		uint8_t ord_bits, uint8_t ov_bits,
		uint8_t const* buffer, uint32_t buffer_size)
{
	uint32_t la = buffer_size / DBL_BLOCK_LEN;
	uint32_t rest_ad = buffer_size % DBL_BLOCK_LEN;

	uint8_t tweak[KEY_LEN + TWEAK_LEN] = {
					// key material
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					// counter
					ord_bits | 0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0};

	memcpy(&tweak[0], &key, KEY_LEN);
	uint8_t crypt[BLOCK_LEN] = {0};
	for (uint32_t i = 0; i < la; i++)
	{
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak[KEY_LEN + TWEAK_LEN - sizeof(unsigned int)], &bigendian, sizeof(unsigned int));

		Deoxys_BC_encrypt_buffer(crypt, tweak, &buffer[i*DBL_BLOCK_LEN], &buffer[i*DBL_BLOCK_LEN+BLOCK_LEN]);

                XOR_block_override(tag, crypt, TAG_LEN);
		memset(crypt, 0, BLOCK_LEN);
	}
	if (rest_ad)
	{
		uint32_t i = la;
		uint8_t tweak_ov[KEY_LEN + TWEAK_LEN] = {
					// key material
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					// counter
					ov_bits | 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					0, 0, 0, 0};
		memcpy(&tweak_ov[0], &key, KEY_LEN);
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak_ov[TWEAK_LEN - sizeof(unsigned int)], &bigendian, sizeof(unsigned int));

		// pad last block with 10*
		uint8_t padded[DBL_BLOCK_LEN] = {0};
		memcpy(padded, &buffer[la*DBL_BLOCK_LEN], rest_ad);
		padded[rest_ad] = 0x80;

		Deoxys_BC_encrypt_buffer_reuse(padded, tweak_ov, padded, &padded[BLOCK_LEN]);
                XOR_block_override(tag, padded, TAG_LEN);
		memset(padded, 0, DBL_BLOCK_LEN);
	}
}

void encrypt_tag(	uint8_t* encrypted_tag,
			uint8_t const* tag,
			uint8_t const* key,
			uint8_t const* nonce)
{
	uint8_t tweak[KEY_LEN + TWEAK_LEN] = 
			{	// key material
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				// constant
				1, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
	memcpy(&tweak[0], &key, KEY_LEN);
	Deoxys_BC_encrypt_buffer(encrypted_tag, tweak, nonce, tag);
}


void compute_tag(uint8_t* tag,	 	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size, 
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t ad_tag[TAG_LEN] = {0};
	uint8_t cipher_tag[TAG_LEN] = {0};

	compute_partial_tag(ad_tag, key, 2, 6, buffer_ad, buffer_ad_size);
	compute_partial_tag(cipher_tag, key,  0, 4, buffer_message, buffer_message_size);

	XOR_block_override(ad_tag, cipher_tag, TAG_LEN);

	encrypt_tag(tag, ad_tag, key, nonce);
}

/**
	message is buffer_message padded with 0s if buffer_message_size is not multiple of BLOCK_LEN
	message is one block of zeroes if buffer_message_size is 0
*/
uint8_t* encrypt_message(uint32_t* ciphertext_size,
			uint8_t const* buffer_message, uint32_t buffer_message_size,
			uint8_t const* key, uint8_t const* tag, uint8_t const* nonce)
{
	uint32_t blocks = buffer_message_size / BLOCK_LEN;
	uint32_t rest_msg = buffer_message_size % BLOCK_LEN;
	uint32_t ceil_buffer_size = blocks * BLOCK_LEN + (rest_msg == 0 ? 0 : 1);

	uint8_t* cipher = calloc(ceil_buffer_size, sizeof(uint8_t));

	uint8_t tweak[KEY_LEN + TWEAK_LEN] = 
			{	// key
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				// counter
				3, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
	memcpy(&tweak[0], &key, KEY_LEN);
	for (int i = 0; i < blocks; i++)
	{
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak[KEY_LEN + TWEAK_LEN - sizeof(unsigned int)], &bigendian, sizeof(unsigned int));

		Deoxys_BC_encrypt_buffer(&cipher[i*BLOCK_LEN], tweak, tag, nonce);
	}
	if (rest_msg)
	{
		uint32_t i = blocks;
		uint8_t tweak_ov[KEY_LEN + TWEAK_LEN] =	
			{	// key
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				// counter
				7, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			};
		memcpy(&tweak_ov[0], &key, KEY_LEN);
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak_ov[TWEAK_LEN - sizeof(unsigned int)], &bigendian, sizeof(unsigned int));

		Deoxys_BC_encrypt_buffer(&cipher[i*BLOCK_LEN], tweak_ov, tag, nonce);
	}

	*ciphertext_size = buffer_message_size;

	return cipher;
}

uint8_t* Deoxys_AE2_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t tag[TAG_LEN];
	compute_tag(tag, key, nonce, buffer_message, buffer_message_size, buffer_ad, buffer_ad_size);

	uint32_t ciphertext_size;
	uint8_t* ciphertext = encrypt_message(&ciphertext_size, buffer_message, buffer_message_size, key, tag, nonce);

	uint8_t* auth_cipher = calloc(ciphertext_size + TAG_LEN, sizeof(uint8_t));
	memcpy(auth_cipher, ciphertext, ciphertext_size);
	memcpy(&auth_cipher[ciphertext_size], tag, TAG_LEN);
	free(ciphertext);

	return auth_cipher;
}

/*
	Decryption probably doesnt yet work
*/
uint8_t* decrypt_message(uint32_t* ciphertext_size,
			uint8_t const* buffer_message, uint32_t buffer_message_size,
			uint8_t const* roundtweakeys, uint8_t const* tag, uint8_t const* nonce)
{
	return encrypt_message(ciphertext_size, buffer_message, buffer_message_size, roundtweakeys, tag, nonce);
}

uint8_t* Deoxys_AE2_decrypt_buffer(	int* authentication_failed, uint32_t* buffer_message_size,
					uint8_t const* key, uint8_t const* nonce, uint8_t const* tag,
					uint8_t const* buffer_ciphertext, uint32_t buffer_ciphertext_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t roundtweakeys[17*16];
	generateRoundTweakeys(roundtweakeys, key);

	uint32_t decrypted_size;
	uint8_t* buffer_decrypted = decrypt_message(&decrypted_size, buffer_ciphertext, buffer_ciphertext_size, roundtweakeys, tag, nonce);

	uint8_t auth_tag[TAG_LEN];
	compute_tag(auth_tag, roundtweakeys, nonce, buffer_decrypted, decrypted_size, buffer_ad, buffer_ad_size);

	int not_authenticated = memcmp(auth_tag, tag, TAG_LEN) == 0 ? 0 : 1;
	*authentication_failed = not_authenticated;

	int authenticated = !not_authenticated;
	*buffer_message_size = 0;
	uint8_t* buffer_message = NULL;
	if (authenticated)
	{
		*buffer_message_size = decrypted_size;
		buffer_message = buffer_decrypted;
	}

	return buffer_message;
}
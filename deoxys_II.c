#include <stdlib.h>
#include <string.h>

#include "deoxys_II.h"
#include "deoxys_bc_384.h"

#define BLOCK_LEN	16 //Block length in Deoxys is 128 bit only
#define TAG_LEN		BLOCK_LEN //Tag length in Deoxys is 128 bit only
#define TWEAK_LEN	16 //Tweak length in Deoxys is 128 bit
#define NONCE_LEN	15 //Nonce length in Deoxys-II is 120 bit

/**
	Computes the xor of first and second bytewise
	both input arrays must be of size BLOCK_LEN
	also deallocates the input arrays 
*/
uint8_t* XOR_block(uint8_t* first, uint8_t* second, int length)
{
	uint8_t* XOR = calloc(length, sizeof(uint8_t));
        for (int i = 0; i < length; i++) {
		XOR[i] = first[i] ^ second[i];
	}

	free(first);
	free(second);

	return XOR;
}

/**
	Computes the xor of first and second bytewise
	both input arrays must be of size BLOCK_LEN
	first argument is overwritten with the result
*/
void XOR_block_override(uint8_t* first, uint8_t* second, int length)
{
        for (int i = 0; i < length; i++) {
		first[i] = first[i] ^ second[i];
	}
}

/**
	computes one stage of tag generation
	deallocates tag
*/
void compute_partial_tag(
		uint8_t* tag,
		uint8_t const* key,
		uint8_t ord_bits, uint8_t ov_bits,
		uint8_t const* buffer, uint32_t buffer_size)
{
	uint32_t la = buffer_size / BLOCK_LEN;
	uint32_t rest_ad = buffer_size % BLOCK_LEN;

	uint8_t tweak[TWEAK_LEN] = {ord_bits | 0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0};

	uint8_t crypt[BLOCK_LEN] = {0};
	for (uint32_t i = 0; i < la; i++)
	{
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak[TWEAK_LEN-4], &bigendian, 4);

		Deoxys_BC_encrypt_buffer(crypt, key, tweak, &buffer[i*BLOCK_LEN]);

                XOR_block_override(tag, crypt, TAG_LEN);
		memset(crypt, 0, BLOCK_LEN);
	}
	if (rest_ad)
	{
		uint32_t i = la;
		uint8_t tweak_ov[TWEAK_LEN] = {ov_bits | 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					0, 0, 0, 0};
		unsigned int bigendian = __builtin_bswap32(i);
		memcpy(&tweak_ov[TWEAK_LEN-4], &bigendian, 4);

		// pad last block with 10*
		uint8_t padded[BLOCK_LEN] = {0};
		memcpy(padded, &buffer[la*BLOCK_LEN], rest_ad);
		padded[rest_ad] = 0x80;

		Deoxys_BC_encrypt_buffer(padded, key, tweak_ov, padded);
                XOR_block_override(tag, padded, TAG_LEN);
		memset(padded, 0, BLOCK_LEN);
	}
}

void encrypt_tag(	uint8_t* encrypted_tag,
			uint8_t const* tag,
			uint8_t const* key, 
			uint8_t const* nonce)
{
	uint8_t tweak[TWEAK_LEN] = 
			{
				 16 | 0, nonce[0], nonce[1], nonce[2],
				nonce[3], nonce[4], nonce[5], nonce[6], 
				nonce[7], nonce[8], nonce[9], nonce[10], 
				nonce[11], nonce[12], nonce[13], nonce[14]
			};
	Deoxys_BC_encrypt_buffer(encrypted_tag, key, tweak, tag);
}

/**
	message is buffer_message padded with 0s if buffer_message_size is not multiple of BLOCK_LEN
	message is one block of zeroes if buffer_message_size is 0
*/
uint8_t* encrypt_message(uint32_t* ciphertext_size,
			uint8_t const* buffer_message, uint32_t buffer_message_size,
			uint8_t const* key, uint8_t const* tag, uint8_t const* nonce)
{
	uint32_t ceiled_div = buffer_message_size != 0 ? 1 + ((buffer_message_size - 1) / BLOCK_LEN) : 0;
	uint32_t ceil_buffer_size = ceiled_div * BLOCK_LEN;

	uint8_t* msg = calloc(ceil_buffer_size, sizeof(uint8_t));
	memcpy(msg, buffer_message, buffer_message_size);

	uint8_t* cipher = calloc(ceil_buffer_size, sizeof(uint8_t));

	uint8_t padded_nonce[BLOCK_LEN] = 
			{
				       0, nonce[0], nonce[1], nonce[2],
				nonce[3], nonce[4], nonce[5], nonce[6], 
				nonce[7], nonce[8], nonce[9], nonce[10], 
				nonce[11], nonce[12], nonce[13], nonce[14]
			};

	for (int i = 0; i < ceiled_div; i++)
	{
		uint8_t tweak[TWEAK_LEN] = 
			{
				 128 | tag[0], tag[1], tag[2], tag[3],
				tag[ 4], tag[ 5], tag[ 6], tag[ 7], 
				tag[ 8], tag[ 9], tag[10], tag[11], 
				tag[12] ^ (i >> 24), tag[13] ^ (i << 8 >> 24), tag[14] ^ (i << 16 >> 24), tag[15] ^ (i << 24 >> 24)
			};

		uint8_t* stream_blk = Deoxys_BC_encrypt(key, tweak, padded_nonce);

		uint8_t* blk = calloc(BLOCK_LEN, sizeof(uint8_t));
		memcpy(blk, &msg[i * BLOCK_LEN], BLOCK_LEN);

		uint8_t* cipher_blk = XOR_block(blk, stream_blk, BLOCK_LEN);



		for (int j = 0; j < BLOCK_LEN; j++)
		{
			cipher[i*BLOCK_LEN + j] = cipher_blk[j];
		}

	}
	free(msg);

	*ciphertext_size = buffer_message_size;

	return cipher;
}

void compute_tag(uint8_t* tag,	 	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size, 
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t ad_tag[TAG_LEN] = {0};
	uint8_t cipher_tag[TAG_LEN] = {0};

	compute_partial_tag(ad_tag, key, 32, 96, buffer_ad, buffer_ad_size);
	compute_partial_tag(cipher_tag, key,  0, 64, buffer_message, buffer_message_size);

	XOR_block_override(ad_tag, cipher_tag, TAG_LEN);

	encrypt_tag(tag, ad_tag, key, nonce);
}

uint8_t* Deoxys_II_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t tag[TAG_LEN];
	compute_tag(tag, key, nonce, buffer_message, buffer_message_size, buffer_ad, buffer_ad_size);

	uint32_t ciphertext_size;
	uint8_t* ciphertext = encrypt_message(&ciphertext_size, buffer_message, buffer_message_size, key, tag, nonce);

	uint8_t* auth_cipher = calloc(ciphertext_size + TAG_LEN, sizeof(uint8_t));
	for (unsigned long long i = 0; i < ciphertext_size; i++)
		auth_cipher[i] = ciphertext[i];
	for (unsigned long long i = 0; i < TAG_LEN; i++)
		auth_cipher[ciphertext_size + i] = tag[i];

	free(ciphertext);

	return auth_cipher;

}

uint8_t* decrypt_message(uint32_t* ciphertext_size,
			uint8_t const* buffer_message, uint32_t buffer_message_size,
			uint8_t const* key, uint8_t const* tag, uint8_t const* nonce)
{
	return encrypt_message(ciphertext_size, buffer_message, buffer_message_size, key, tag, nonce);
}

uint8_t* Deoxys_II_decrypt_buffer(	int* authentication_failed, uint32_t* buffer_message_size,
					uint8_t const* key, uint8_t const* nonce, uint8_t const* tag,
					uint8_t const* buffer_ciphertext, uint32_t buffer_ciphertext_size,
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint32_t decrypted_size;
	uint8_t* buffer_decrypted = decrypt_message(&decrypted_size, buffer_ciphertext, buffer_ciphertext_size, key, tag, nonce);

	uint8_t auth_tag[TAG_LEN];
	compute_tag(auth_tag, key, nonce, buffer_decrypted, decrypted_size, buffer_ad, buffer_ad_size);

	int not_authenticated = memcmp(auth_tag, tag, TAG_LEN) == 0 ? 0 : 1;
	*authentication_failed = not_authenticated;

	int authenticated = !not_authenticated;
	*buffer_message_size = 0;
	uint8_t* buffer_message = NULL;
	if (authenticated)
	{
		*buffer_message_size = decrypted_size;
		buffer_message = calloc(decrypted_size, sizeof(uint8_t));
		memcpy(buffer_message, buffer_decrypted, decrypted_size);
	}
	free(buffer_decrypted);

	return buffer_message;
}

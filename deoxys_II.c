#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoxys_II.h"
#include "deoxys_bc_384.h"

/**
	buffer contains the message to be padded
	buffer_len must be greater than zero and less than BLOCK_LEN
*/
uint8_t* pad10(uint8_t const* buffer, uint32_t buffer_len)
{
	uint8_t* padded = calloc(BLOCK_LEN, sizeof(uint8_t));
	memcpy(padded, buffer, buffer_len);
	padded[buffer_len] = 128;
	return padded;
}

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
	computes one stage of tag generation
	deallocates tag
*/
uint8_t* compute_partial_tag(
		uint8_t const* tag,
		uint8_t const* key,
		uint8_t ord_bits, uint8_t ov_bits,
		uint8_t const* buffer, uint32_t buffer_size)
{
	uint32_t la = buffer_size / BLOCK_LEN;
	uint32_t rest_ad = buffer_size % BLOCK_LEN;
	uint8_t* tag_tmp = calloc(TAG_LEN, sizeof(uint8_t));
	memcpy(tag_tmp, tag, TAG_LEN);


	for (uint32_t i = 0; i < la; i++)
	{
		uint8_t tweak[TWEAK_LEN] = {ord_bits | 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					i >> 24, i << 8 >> 24, i << 16 >> 24, i << 24 >> 24};


		uint8_t* enc = Deoxys_BC_encrypt(key, tweak, &buffer[i*BLOCK_LEN]);

                tag_tmp = XOR_block(tag_tmp, enc, TAG_LEN);
	}
	if (rest_ad)
	{
		uint32_t i = la;
		uint8_t tweak[TWEAK_LEN] = {ov_bits | 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					i >> 24, i << 8 >> 24, i << 16 >> 24, i << 24 >> 24};

		uint8_t* padded = pad10(&buffer[la*BLOCK_LEN], rest_ad);
		uint8_t* enc = Deoxys_BC_encrypt(key, tweak, padded);
		tag_tmp = XOR_block(tag_tmp, enc, TAG_LEN);
		free(padded);
	}

	return tag_tmp;
}

uint8_t* encrypt_tag(	uint8_t const* tag, 
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
	return Deoxys_BC_encrypt(key, tweak, tag);
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


uint8_t* Deoxys_II_encrypt_buffer(	uint8_t const* key, uint8_t const* nonce,
					uint8_t const* buffer_message, uint32_t buffer_message_size, 
					uint8_t const* buffer_ad, uint32_t buffer_ad_size)
{
	uint8_t* null_tag = calloc(TAG_LEN, sizeof(uint8_t));

	uint8_t* tag1 = compute_partial_tag(null_tag, key, 32, 96, buffer_ad, buffer_ad_size);
	uint8_t* tag2 = compute_partial_tag(null_tag, key,  0, 64, buffer_message, buffer_message_size);
	free(null_tag);

	uint8_t* xor_tag = XOR_block(tag1, tag2, TAG_LEN);

	uint8_t* tag = encrypt_tag(xor_tag, key, nonce);
	free(xor_tag);

	printf("\ntag: \n");
	for (int i = 0; i < TAG_LEN; i++) {
		printf("%x ", tag[i]);
	}
	printf("\nad: \n");
	uint32_t ciphertext_size;
	uint8_t* ciphertext = encrypt_message(&ciphertext_size, buffer_message, buffer_message_size, key, tag, nonce);

	printf("Ciphertext_size %i\n", ciphertext_size);


	uint8_t* auth_cipher = calloc(ciphertext_size + TAG_LEN, sizeof(uint8_t));
	for (unsigned long long i = 0; i < ciphertext_size; i++)
		auth_cipher[i] = ciphertext[i];
	for (unsigned long long i = 0; i < TAG_LEN; i++)
		auth_cipher[ciphertext_size + i] = tag[i];

	free(ciphertext);
	free(tag);

	return auth_cipher;

}




int main()
{
	uint8_t* key = calloc(32, sizeof(uint8_t));
	uint8_t arr_key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	memcpy(key, arr_key, 32);

	uint8_t* nonce = calloc(15, sizeof(uint8_t));
	uint8_t arr_nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	memcpy(nonce, arr_nonce, 15);
	uint8_t* message = calloc(0, sizeof(uint8_t));
	uint8_t* ad = calloc(0, sizeof(uint8_t));
	printf("Key: \n");
	for (int i = 0; i < 32; i++) {
		printf("%i", key[i]);
	}
	printf("\nNonce: \n");
	for (int i = 0; i < 15; i++) {
		printf("%i", nonce[i]);
	}
	printf("\nmessage: \n");
	for (int i = 0; i < 0; i++) {
		printf("%i", message[i]);
	}
	printf("\nad: \n");
	for (int i = 0; i < 0; i++) {
		printf("%i", ad[i]);
	}
	printf("\n");
	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 0, ad, 0);

	printf("Cipher result:\n");
	for (int i = 0; i < TAG_LEN; i++) {
		printf("%x ", crypt[i]);
	}
	printf("\nShould be: \n");
	printf("2b 97 bd 77 71 2f 0c de 97 53 09 95 9d fe 1d 7c\n");

	free(crypt);
	return 0;
}

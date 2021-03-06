#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoxys_II.h"

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
uint8_t* XOR_block(uint8_t* first, uint8_t* second)
{
	uint8_t* XOR = calloc(BLOCK_LEN, sizeof(uint8_t));
        for (int i = 0; i < BLOCK_LEN; i++)
		XOR[i] = first[i] ^ second[i];

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
		uint8_t tweak[TWEAK_LEN] = {ord_bits || 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					i >> 24, i << 8 >> 24, i << 16 >> 24, i << 24 >> 24};

		uint8_t* enc = Deoxys_BC_encrypt(key, tweak, &buffer[i*BLOCK_LEN]);
                tag_tmp = XOR_block(tag_tmp, enc);
	}
	if (rest_ad)
	{
		uint32_t i = la;
		uint8_t tweak[TWEAK_LEN] = {ov_bits || 0, 0, 0, 0,
					0, 0, 0, 0, 
					0, 0, 0, 0,
					i >> 24, i << 8 >> 24, i << 16 >> 24, i << 24 >> 24};

		uint8_t* padded = pad10(&buffer[la*BLOCK_LEN], rest_ad);
		uint8_t* enc = Deoxys_BC_encrypt(key, tweak, padded);
		tag_tmp = XOR_block(tag_tmp, enc);
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
				 16 || 0, nonce[0], nonce[1], nonce[2],
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
	uint32_t ceiled_div = 1 + ((buffer_message_size - 1) / BLOCK_LEN);
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
				 128 || tag[0] % 128, tag[1], tag[2], tag[3],
				tag[ 4], tag[ 5], tag[ 6], tag[ 7], 
				tag[ 8], tag[ 9], tag[10], tag[11], 
				tag[12] ^ (i >> 24), tag[13] ^ (i << 8 >> 24), tag[14] ^ (i << 16 >> 24), tag[15] ^ (i << 24 >> 24)
			};
		uint8_t* stream_blk = Deoxys_BC_encrypt(key, tweak, padded_nonce);

		uint8_t* blk = calloc(BLOCK_LEN, sizeof(uint8_t));
		memcpy(blk, &msg[i * BLOCK_LEN], BLOCK_LEN);

		uint8_t* cipher_blk = XOR_block(blk, stream_blk);
		for (int j = 0; j < BLOCK_LEN; j++)
		{
			cipher[i*BLOCK_LEN + j] = cipher_blk[j];
		}
		free(blk);
	}
	free(msg);

	*ciphertext_size = ceil_buffer_size;
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

	uint8_t* xor_tag = XOR_block(tag1, tag2);

	uint8_t* tag = encrypt_tag(xor_tag, key, nonce);
	free(xor_tag);

	uint32_t ciphertext_size;
	uint8_t* ciphertext = encrypt_message(&ciphertext_size, buffer_message, buffer_message_size, key, tag, nonce);

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
	return 0;
}

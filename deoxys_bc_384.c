#include <stdlib.h>
#include <string.h>

#include "tiny_aes.h"
#include "deoxys_bc_384.h"

#define TWEAKEY_SIZE 16 //tweakey is split into 3 parts of 128bit
#define BLOCK_LEN 16 //Block length in Deoxys is 128 bit only
#define ROUNDS_NUM 16 //There are 16 AES-like rounds
#define INTERNAL_STATE_SIZE 16 //There are 16 bytes in the state

static const uint8_t h[16] = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};

// The round constant word array, Rcon[i], contains the values given by
// RCON values as given in table A.3
static const uint8_t RCON[17] = {
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72 };


/**
	Computes the xor of first and second bytewise
	both input arrays must be of size BLOCK_LEN
	also deallocates the input arrays 
*/
uint8_t* XOR_block_nonfree(uint8_t* first, uint8_t* second, uint8_t* third, uint8_t* fourth)
{
	uint8_t* XOR = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
        for (int i = 0; i < INTERNAL_STATE_SIZE; i++)
		XOR[i] = first[i] ^ second[i] ^ third[i] ^ fourth[i];
	return XOR;
}

uint8_t* calculate_subtweakey(uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, uint8_t* RCi) {
	return XOR_block_nonfree(tk1, tk2, tk3, RCi);

}

uint8_t* getRC(int i) {
	uint8_t* RCi = calloc(16, sizeof(uint8_t));
	uint8_t tmp[16] = {1, 2, 4, 8, RCON[i], RCON[i], RCON[i], RCON[i], 0, 0, 0, 0, 0, 0, 0, 0};
	memcpy(RCi, tmp, 16);
	return RCi;
}

void add_subtweakey(state_t* internal_state, uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, int i) {
	uint8_t* RCi = getRC(i);
	uint8_t* subtweakey = calculate_subtweakey(tk1, tk2, tk3, RCi);
	free(RCi);

	AddRoundKey(internal_state, subtweakey);
	free(subtweakey);
}

uint8_t* nextTK1(uint8_t* prevTK1) {
	uint8_t* newTK1 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK1[i] = prevTK1[h[i]];
	}
	free(prevTK1);

	return newTK1;
}

uint8_t* nextTK2(uint8_t* prevTK2) {

	for (int j = 0; j < TWEAKEY_SIZE; j++) {
		prevTK2[j] = (prevTK2[j] << 1) | (((prevTK2[j] & 0x80) >> 7) ^ ((prevTK2[j] & 0x20) >> 5));
	}

	uint8_t* newTK2 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK2[i] = prevTK2[h[i]];
	}
	free(prevTK2);

	return newTK2;
}

uint8_t* nextTK3(uint8_t* prevTK3) {

	for (int j = 0; j < TWEAKEY_SIZE; j++) {
		prevTK3[j] = (prevTK3[j] >> 1) | (((prevTK3[j] & 0x01) << 7) ^ ((prevTK3[j] & 0x40) << 1));
	}

	uint8_t* newTK3 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK3[i] = prevTK3[h[i]];
	}
	free(prevTK3);

	return newTK3;
}


uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext) {

	state_t* internal_state = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
	memcpy(internal_state, plaintext, BLOCK_LEN);

	uint8_t* tk1 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));
	uint8_t* tk2 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));
	uint8_t* tk3 = calloc(TWEAKEY_SIZE, sizeof(uint8_t));

	memcpy(tk3, key, TWEAKEY_SIZE);
	memcpy(tk2, &key[TWEAKEY_SIZE], TWEAKEY_SIZE);
	memcpy(tk1, tweak, TWEAKEY_SIZE);

	for (int i = 0; i < ROUNDS_NUM; i++) {

		add_subtweakey(internal_state, tk1, tk2, tk3, i);
		SubBytes(internal_state);
		ShiftRows(internal_state);
		MixColumns(internal_state);

		tk1 = nextTK1(tk1);
		tk2 = nextTK2(tk2);
		tk3 = nextTK3(tk3);
	}
	add_subtweakey(internal_state, tk1, tk2, tk3, ROUNDS_NUM);

	free(tk3);
	free(tk2);
	free(tk1);

	uint8_t* ret = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
	memcpy(ret, internal_state, INTERNAL_STATE_SIZE);
	free(internal_state);
	return ret;
}
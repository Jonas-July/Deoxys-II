#include <stdlib.h>
#include <string.h>

#include "tiny_aes.h"
#include "deoxys_bc_384.h"

#define TWEAKEY_SIZE 16 //tweakey is split into 3 parts of 128bit
#define BLOCK_LEN 16 //Block length in Deoxys is 128 bit only
#define ROUNDS_NUM 16 //There are 16 AES-like rounds
#define INTERNAL_STATE_SIZE 16 //There are 16 bytes in the state

// The round constant word array, Rcon[i], contains the values given by
// RCON values as given in table A.3
static const uint8_t RCON[17] = {
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72 };

#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1

typedef uint8_t v16si __attribute__ ((vector_size (16)));
static const v16si h = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};

#else

static const uint8_t h[16] = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};

#endif // GCC_VECTOR_EXTENSIONS Definitions

/**
	Computes the xor of the first three inputs with the round constant
	Loop is unrolled since most entries in the round constant stay constant between rounds
*/
#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1

v16si XOR4_subtweakey(uint8_t* first, uint8_t* second, uint8_t* third, uint8_t constant)
{

	v16si RCON = {1, 2, 4, 8, constant, constant, constant, constant, 0, 0, 0, 0, 0, 0, 0, 0};
	v16si res = (*((v16si*)first)) ^ (*((v16si*)second)) ^ (*((v16si*)third)) ^ RCON;

	return res;
}

static v16si calculate_subtweakey(uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, int round_index) {
	return XOR4_subtweakey(tk1, tk2, tk3, RCON[round_index]);

}

void add_subtweakey(state_t* internal_state, uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, int i) {
	v16si subtweakey = calculate_subtweakey(tk1, tk2, tk3, i);

	AddRoundKey(internal_state, (uint8_t*)(&subtweakey));
}

#else
uint8_t* XOR4_subtweakey(uint8_t* first, uint8_t* second, uint8_t* third, uint8_t constant, uint8_t* result_buffer)
{
	result_buffer[0] = first[0] ^ second[0] ^ third[0] ^ 1;
	result_buffer[1] = first[1] ^ second[1] ^ third[1] ^ 2;
	result_buffer[2] = first[2] ^ second[2] ^ third[2] ^ 4;
	result_buffer[3] = first[3] ^ second[3] ^ third[3] ^ 8;

        for (int i = 4; i < 8; i++)
		result_buffer[i] = first[i] ^ second[i] ^ third[i] ^ constant;

        for (int i = 8; i < 16; i++)
		result_buffer[i] = first[i] ^ second[i] ^ third[i];

	return result_buffer;
}

uint8_t* calculate_subtweakey(uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, int round_index, uint8_t* result_buffer) {
	return XOR4_subtweakey(tk1, tk2, tk3, RCON[round_index], result_buffer);
}

void add_subtweakey(state_t* internal_state, uint8_t* tk1, uint8_t* tk2, uint8_t* tk3, int i) {
	uint8_t subtweakey[INTERNAL_STATE_SIZE];
	calculate_subtweakey(tk1, tk2, tk3, i, subtweakey);

	AddRoundKey(internal_state, subtweakey);
}

#endif // XOR4_subtweakey

#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1

uint8_t* nextTK1(uint8_t* prevTK1) {
	v16si vec_newTK1 = *((v16si*)prevTK1);
	v16si res;

	res = __builtin_shuffle (vec_newTK1, h);
	memcpy(prevTK1, &res, 16);
	return prevTK1;
}

uint8_t* nextTK2(uint8_t* prevTK2) {
	v16si vec_newTK2 = *((v16si*)prevTK2);

	vec_newTK2 = (vec_newTK2 << 1) | (((vec_newTK2 & 0x80) >> 7) ^ ((vec_newTK2 & 0x20) >> 5));
	v16si res;

	res = __builtin_shuffle (vec_newTK2, h);
	memcpy(prevTK2, &res, 16);
	return prevTK2;
}

uint8_t* nextTK3(uint8_t* prevTK3) {
	v16si vec_newTK3 = *((v16si*)prevTK3);

	vec_newTK3 = (vec_newTK3 >> 1) | (((vec_newTK3 & 0x01) << 7) ^ ((vec_newTK3 & 0x40) << 1));
	v16si res;

	res = __builtin_shuffle (vec_newTK3, h);
	memcpy(prevTK3, &res, 16);
	return prevTK3;
}



#else

uint8_t* nextTK1(uint8_t* prevTK1) {
	uint8_t newTK1[TWEAKEY_SIZE];
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK1[i] = prevTK1[h[i]];
	}
	memcpy(prevTK1, newTK1, TWEAKEY_SIZE);

	return prevTK1;
}

uint8_t* nextTK2(uint8_t* prevTK2) {

	for (int j = 0; j < TWEAKEY_SIZE; j++) {
		prevTK2[j] = (prevTK2[j] << 1) | (((prevTK2[j] & 0x80) >> 7) ^ ((prevTK2[j] & 0x20) >> 5));
	}

	uint8_t newTK2[TWEAKEY_SIZE];
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK2[i] = prevTK2[h[i]];
	}
	memcpy(prevTK2, newTK2, TWEAKEY_SIZE);

	return prevTK2;
}

uint8_t* nextTK3(uint8_t* prevTK3) {

	for (int j = 0; j < TWEAKEY_SIZE; j++) {
		prevTK3[j] = (prevTK3[j] >> 1) | (((prevTK3[j] & 0x01) << 7) ^ ((prevTK3[j] & 0x40) << 1));
	}

	uint8_t newTK3[TWEAKEY_SIZE];
	for (int i = 0; i < TWEAKEY_SIZE; i++) {
		newTK3[i] = prevTK3[h[i]];
	}
	memcpy(prevTK3, newTK3, TWEAKEY_SIZE);

	return prevTK3;
}


#endif // GCC_VECTOR_EXTENSIONS for next tweakeys

#if defined(USE_AES_NI) && USE_AES_NI == 1
#include <wmmintrin.h>
uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext) {
	__m128i m = _mm_loadu_si128((__m128i *) plaintext);
	uint8_t* zero = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
	__m128i z = _mm_loadu_si128((__m128i *) zero);

	uint8_t tk1[TWEAKEY_SIZE];
	uint8_t tk2[TWEAKEY_SIZE];
	uint8_t tk3[TWEAKEY_SIZE];

	memcpy(tk3, key, TWEAKEY_SIZE);
	memcpy(tk2, &key[TWEAKEY_SIZE], TWEAKEY_SIZE);
	memcpy(tk1, tweak, TWEAKEY_SIZE);

	for (int i = 0; i < ROUNDS_NUM; i++) {

		state_t* s = &m;
		add_subtweakey(s, tk1, tk2, tk3, i);
		m = _mm_aesenc_si128(m, z);

		nextTK1(tk1);
		nextTK2(tk2);
		nextTK3(tk3);
	}
	state_t* s = &m;
	add_subtweakey(s, tk1, tk2, tk3, ROUNDS_NUM);

	uint8_t* ret = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
	_mm_storeu_si128((__m128i *) ret, m);

	return ret;
}

#else

uint8_t* Deoxys_BC_encrypt(uint8_t const* key, uint8_t const* tweak, uint8_t const* plaintext) {

	state_t internal_state[1]; // pointer is a bit easier to handle
	memcpy(internal_state, plaintext, BLOCK_LEN);

	uint8_t tk1[TWEAKEY_SIZE];
	uint8_t tk2[TWEAKEY_SIZE];
	uint8_t tk3[TWEAKEY_SIZE];

	memcpy(tk3, key, TWEAKEY_SIZE);
	memcpy(tk2, &key[TWEAKEY_SIZE], TWEAKEY_SIZE);
	memcpy(tk1, tweak, TWEAKEY_SIZE);

	for (int i = 0; i < ROUNDS_NUM; i++) {

		add_subtweakey(internal_state, tk1, tk2, tk3, i);
		SubBytes(internal_state);
		ShiftRows(internal_state);
		MixColumns(internal_state);

		nextTK1(tk1);
		nextTK2(tk2);
		nextTK3(tk3);
	}
	add_subtweakey(internal_state, tk1, tk2, tk3, ROUNDS_NUM);

	uint8_t* ret = calloc(INTERNAL_STATE_SIZE, sizeof(uint8_t));
	memcpy(ret, internal_state, INTERNAL_STATE_SIZE);

	return ret;
}
#endif // USE_AES_NI

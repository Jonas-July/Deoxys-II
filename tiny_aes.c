#include <string.h>
#include <stdlib.h>

#include "tiny_aes.h"

/**
	This file contains standard AES functions
	taken from tiny-aes AES implementation
	by kokke, available on GitHub
	https://github.com/kokke/tiny-AES-c
*/

#if defined(USE_LOOKUP_TABLE) && USE_LOOKUP_TABLE == 1

#define getSBoxValue(num) (sbox[(num)])

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM - 
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

#else

#define getSBoxValue(num) (sbox_calculation( (num) ))

// Based on the algorithm described in https://eprint.iacr.org/2015/763.pdf
// Local version is available under:	fast_hardware_inversion.pdf
static const uint8_t sbox_calculation(uint8_t input) {

  // Isomorphic transformation
  uint8_t input_bit[8] = {	(input & (0x01 << 0)) >> 0, (input & (0x01 << 1)) >> 1, (input & (0x01 << 2)) >> 2, (input & (0x01 << 3)) >> 3,
				  (input & (0x01 << 4)) >> 4, (input & (0x01 << 5)) >> 5, (input & (0x01 << 6)) >> 6, (input & (0x01 << 7)) >> 7 };

  uint8_t val[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  val[0] = (input_bit[1] ^ input_bit[3] ^ input_bit[4] ^ input_bit[5]);
  val[1] = (input_bit[0] ^ input_bit[2] ^ input_bit[6] ^ input_bit[7]);
  val[2] = (input_bit[0] ^ input_bit[3] ^ input_bit[7]               );
  val[3] = (input_bit[5]                                             );
  val[4] = (input_bit[1] ^ input_bit[2] ^ input_bit[4] ^ input_bit[5]);
  val[5] = (input_bit[0] ^ input_bit[2] ^ input_bit[4]               );
  val[6] = (input_bit[0] ^ input_bit[1] ^ input_bit[2] ^ input_bit[7]);
  val[7] = (input_bit[2] ^ input_bit[3] ^ input_bit[7]               );


  //Preparation
  uint8_t l[5] = {0, val[0], val[1], val[2], val[3]};
  uint8_t h[5] = {0, val[4], val[5], val[6], val[7]};


  // Stage 1
  uint8_t L[5][5] = {   {0, 0, 0, 0, 0},
			{0, 0,		  l[1] ^ l[2], l[1] ^ l[3], l[1] ^ l[4]},
			{0, 0, 0,  	     l[2] ^ l[3], l[2] ^ l[4]},
			{0, 0, 0, 0,  	  l[3] ^ l[4]},
			{0, 0, 0, 0, 0} };
  uint8_t H[5][5] = {   {0, 0, 0, 0, 0},
			{0, 0,		  h[1] ^ h[2], h[1] ^ h[3], h[1] ^ h[4]},
			{0, 0, 0,  	     h[2] ^ h[3], h[2] ^ h[4]},
			{0, 0, 0, 0,  	  h[3] ^ h[4]},
			{0, 0, 0, 0, 0} };

  uint8_t d[5] = {0, 0, 0, 0, 0};
  d[0] = (H[1][2] | L[1][2]) ^ (H[3][4] | L[3][4]) ^    (h[2] | l[2])    ^ (h[3] & l[3]);
  d[1] = (H[1][2] | L[1][2]) ^ (H[1][3] & L[1][3]) ^    (h[3] | l[3])    ^ (h[4] | l[4]);
  d[2] = (H[1][3] | L[1][3]) ^ (H[1][4] & L[1][4]) ^ (H[2][3] | L[2][3]) ^ (h[4] | l[4]);
  d[3] = (H[1][4] | L[1][4]) ^ (H[2][3] | L[2][3]) ^ (H[2][4] & L[2][4]) ^ (h[1] | l[1]);
  d[4] = (H[2][4] | L[2][4]) ^ (H[3][4] | L[3][4]) ^    (h[1] | l[1])    ^ (h[2] & l[2]);

  // Stage 2
  uint8_t e[5] = {0, 0, 0, 0, 0};
  e[0] = (d[1] | d[4]) & (d[2] | d[3]);
  e[1] = ((d[4] ^ 1) & (d[1] ^ d[2])) | ((d[0] & d[4]) & (d[2] | d[3]));
  e[2] = ((d[3] ^ 1) & (d[2] ^ d[4])) | ((d[0] & d[3]) & (d[1] | d[4]));
  e[3] = ((d[2] ^ 1) & (d[1] ^ d[3])) | ((d[0] & d[2]) & (d[1] | d[4]));
  e[4] = ((d[1] ^ 1) & (d[3] ^ d[4])) | ((d[0] & d[1]) & (d[2] | d[3]));

  // Stage 3
  uint8_t F[5][5] = { {0,		 e[0] ^ e[1], e[0] ^ e[2], e[0] ^ e[3], e[0] ^ e[4]},
		      {0, 0,	      e[1] ^ e[2], e[1] ^ e[3], e[1] ^ e[4]},
		      {0, 0, 0,	   e[2] ^ e[3], e[2] ^ e[4]},
		      {0, 0, 0, 0,	e[3] ^ e[4]},
		      {0, 0, 0, 0, 0} };


  uint8_t res_h[5] = {0, 0, 0, 0, 0};
  res_h[0] = (L[1][4] & F[1][4]) ^ (L[2][3] & F[2][3]);
  res_h[1] = (   l[1] & F[0][1]) ^ (L[2][4] & F[2][4]);
  res_h[2] = (   l[2] & F[0][2]) ^ (L[3][4] & F[3][4]);
  res_h[3] = (   l[3] & F[0][3]) ^ (L[1][2] & F[1][2]);
  res_h[4] = (   l[4] & F[0][4]) ^ (L[1][3] & F[1][3]);

  uint8_t res_l[5] = {0, 0, 0, 0, 0};
  res_l[0] = (H[1][4] & F[1][4]) ^ (H[2][3] & F[2][3]);
  res_l[1] = (   h[1] & F[0][1]) ^ (H[2][4] & F[2][4]);
  res_l[2] = (   h[2] & F[0][2]) ^ (H[3][4] & F[3][4]);
  res_l[3] = (   h[3] & F[0][3]) ^ (H[1][2] & F[1][2]);
  res_l[4] = (   h[4] & F[0][4]) ^ (H[1][3] & F[1][3]);

  // Final transformation back to AES with matrix multiplication
  uint8_t result = 0x00;
  result |= (res_l[0] ^ res_l[1] ^ res_l[2] ^ res_l[3]  ^ res_h[0] ^ res_h[3]                      ) << 0;
  result |= (res_l[2] ^ res_l[3]			^ res_h[0] ^ res_h[1] ^ res_h[2] ^ res_h[4]) << 1;
  result |= (res_l[0] ^ res_l[1] ^ res_l[2] ^ res_l[3]  ^ res_h[1] ^ res_h[3]                      ) << 2;
  result |= (res_l[0] ^ res_l[4]			^ res_h[0] ^ res_h[1] ^ res_h[2] ^ res_h[4]) << 3;
  result |= (res_l[0] ^ res_l[1] ^ res_l[3] ^ res_l[4]  ^ res_h[0] ^ res_h[4]                      ) << 4;
  result |= (res_l[0] ^ res_l[4]			^ res_h[0] ^ res_h[2] ^ res_h[3] ^ res_h[4]) << 5;
  result |= (res_l[2] ^ res_l[4]			^ res_h[2] ^ res_h[4]		      ) << 6;
  result |= (res_l[0] ^ res_l[2]			^ res_h[0] ^ res_h[1] ^ res_h[3] ^ res_h[4]) << 7;

  return result ^ 0x63;
}

#endif // USE_LOOKUP_TABLE SBox definition

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
#if defined(GCC_VECTOR_EXTENSIONS) && GCC_VECTOR_EXTENSIONS == 1
v16si AddRoundKey(v16si state, const v16si RoundKey)
{
  return state ^ RoundKey;
}

#else

void AddRoundKey(state_t* state, const uint8_t* RoundKey)
{
  uint8_t i,j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[i*4 + j];
    }
  }
}

#endif // GCC_VECTOR_EXTENSIONS

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void SubBytes(state_t* state)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = getSBoxValue((*state)[j][i]);
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void ShiftRows(state_t* state)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}


static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
void MixColumns(state_t* state)
{
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {  
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoxys_II.h"

/*
	Test cases are based on the ones used for the Go implementation by oasisprotocol:
	https://github.com/oasisprotocol/deoxysii
	They are called official, but I wasn't able to find a source for that claim
	The paper itself does not provide test cases
*/


// Test vector 1
int test_case_no_message_no_ad() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {};
	uint8_t ad[] = {};
	uint8_t expected[] = {0x2b, 0x97, 0xbd, 0x77, 0x71, 0x2f, 0x0c, 0xde, 0x97, 0x53, 0x09, 0x95, 0x9d, 0xfe, 0x1d, 0x7c};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 0, ad, 0);
	int failed = memcmp(crypt, expected, 16);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 2
int test_case_no_message_with_ad() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {};
	uint8_t ad[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
	uint8_t expected[] = {0x54, 0x70, 0x8a, 0xe5, 0x56, 0x5a, 0x71, 0xf1, 0x47, 0xbd, 0xb9, 0x4d, 0x7b, 0xa3, 0xae, 0xd7};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 0, ad, 32);
	int failed = memcmp(crypt, expected, 16);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 3
int test_case_no_message_with_ad2() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {};
	uint8_t ad[] = {0xf4, 0x95, 0xc9, 0xc0, 0x3d, 0x29, 0x98, 0x96, 0x95, 0xd9, 0x8f, 0xf5, 0xd4, 0x30, 0x65, 0x01, 0x25, 0x80, 0x5c, 0x1e, 0x05, 0x76, 0xd0, 0x6f, 0x26, 0xcb, 0xda, 0x42, 0xb1, 0xf8, 0x22, 0x38, 0xb8};
	uint8_t expected[] = {0x32, 0x77, 0x68, 0x9d, 0xc4, 0x20, 0x8c, 0xc1, 0xff, 0x59, 0xd1, 0x54, 0x34, 0xa1, 0xba, 0xf1};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 0, ad, 33);
	int failed = memcmp(crypt, expected, 16);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 4
int test_case_with_message_no_ad() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
	uint8_t ad[] = {};
	uint8_t expected[] = {0x9d, 0xa2, 0x0d, 0xb1, 0xc2, 0x78, 0x1f, 0x66, 0x69, 0x25, 0x7d, 0x87, 0xe2, 0xa4, 0xd9, 0xbe, 0x19, 0x70, 0xf7, 0x58, 0x1b, 0xef, 0x2c, 0x99, 0x5e, 0x11, 0x49, 0x33, 0x1e, 0x5e, 0x8c, 0xc1, 0x92, 0xce, 0x3a, 0xec, 0x3a, 0x4b, 0x72, 0xff, 0x9e, 0xab, 0x71, 0xc2, 0xa9, 0x34, 0x92, 0xfa};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 32, ad, 0);
	int failed = memcmp(crypt, expected, 48);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 5
int test_case_with_message_no_ad2() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {0x15, 0xcd, 0x77, 0x73, 0x2f, 0x9d, 0x0c, 0x4c, 0x6e, 0x58, 0x1e, 0xf4, 0x00, 0x87, 0x6a, 0xd9, 0x18, 0x8c, 0x5b, 0x88, 0x50, 0xeb, 0xd3, 0x82, 0x24, 0xda, 0x95, 0xd7, 0xcd, 0xc9, 0x9f, 0x7a, 0xcc};
	uint8_t ad[] = {};
	uint8_t expected[] = {0xe5, 0xff, 0xd2, 0xab, 0xc5, 0xb4, 0x59, 0xa7, 0x36, 0x67, 0x75, 0x6e, 0xda, 0x64, 0x43, 0xed, 0xe8, 0x6c, 0x08, 0x83, 0xfc, 0x51, 0xdd, 0x75, 0xd2, 0x2b, 0xb1, 0x49, 0x92, 0xc6, 0x84, 0x61, 0x8c, 0x5f, 0xa7, 0x8d, 0x57, 0x30, 0x8f, 0x19, 0xd0, 0x25, 0x20, 0x72, 0xee, 0x39, 0xdf, 0x5e, 0xcc};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 33, ad, 0);
	int failed = memcmp(crypt, expected, 49);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 6
int test_case_with_message_with_ad() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};
	uint8_t ad[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	uint8_t expected[] = {0x10, 0x9f, 0x8a, 0x16, 0x8b, 0x36, 0xdf, 0xad, 0xe0, 0x26, 0x28, 0xa9, 0xe1, 0x29, 0xd5, 0x25, 0x7f, 0x03, 0xcc, 0x79, 0x12, 0xae, 0xfa, 0x79, 0x72, 0x9b, 0x67, 0xb1, 0x86, 0xa2, 0xb0, 0x8f, 0x65, 0x49, 0xf9, 0xbf, 0x10, 0xac, 0xba, 0x0a, 0x45, 0x1d, 0xbb, 0x24, 0x84, 0xa6, 0x0d, 0x90};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 32, ad, 16);
	int failed = memcmp(crypt, expected, 48);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 7
int test_case_with_message_with_ad2() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {0x42, 0x28, 0x57, 0xfb, 0x16, 0x5a, 0xf0, 0xa3, 0x5c, 0x03, 0x19, 0x9f, 0xb8, 0x95, 0x60, 0x4d, 0xca, 0x9c, 0xea, 0x6d, 0x78, 0x89, 0x54, 0x96, 0x2c, 0x41, 0x9e, 0x0d, 0x5c, 0x22, 0x5c, 0x03, 0x27};
	uint8_t ad[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
	uint8_t expected[] = {0x7d, 0x77, 0x22, 0x03, 0xfa, 0x38, 0xbe, 0x29, 0x6d, 0x8d, 0x20, 0xd8, 0x05, 0x16, 0x31, 0x30, 0xc6, 0x9a, 0xba, 0x8c, 0xb1, 0x6e, 0xd8, 0x45, 0xc2, 0x29, 0x6c, 0x61, 0xa8, 0xf3, 0x4b, 0x39, 0x4e, 0x0b, 0x3f, 0x10, 0xe3, 0x93, 0x3c, 0x78, 0x19, 0x0b, 0x24, 0xb3, 0x30, 0x08, 0xbf, 0x80, 0xe9};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 33, ad, 17);
	int failed = memcmp(crypt, expected, 49);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

// Test vector 7
int test_case_extreme() {
	uint8_t key[32] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};
	uint8_t nonce[15] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e};
	uint8_t message[] = {0x83, 0xda, 0xb2, 0x3b, 0x13, 0x79, 0xe0, 0x90, 0x75, 0x5c, 0x99, 0x07, 0x9c, 0xfe, 0x91, 0x8c, 0xb7, 0x37, 0xe9, 0x89, 0xf2, 0xd7, 0x20, 0xcc, 0xaf, 0xf4, 0x93, 0xa7, 0x44, 0x92, 0x76, 0x44, 0xfe, 0xc3, 0x65, 0x32, 0x11, 0xfa, 0x75, 0x30, 0x6a, 0x83, 0x48, 0x6e, 0x5c, 0x34, 0xec, 0xfe, 0x63, 0x87, 0x0c, 0x97, 0x25, 0x1a, 0x73, 0xe4, 0xb9, 0x03, 0x3a, 0xe3, 0x74, 0x80, 0x97, 0x11, 0xb2, 0x11, 0xed, 0x5d, 0x29, 0x3a, 0x59, 0x2e, 0x46, 0x6a, 0x81, 0x17, 0x0f, 0x1d, 0x85, 0x75, 0x0b, 0x5c, 0xa0, 0x25, 0xcc, 0xd4, 0x57, 0x99, 0x47, 0xed, 0xba, 0xe9, 0xec, 0x13, 0x2b, 0xfb, 0x1a, 0x72, 0x33, 0xad, 0x79, 0xfa, 0xe3, 0x00, 0x06, 0xa6, 0x69, 0x9f, 0x14, 0x38, 0x93, 0x86, 0x1b, 0x97, 0x52, 0x26, 0xed, 0x9d, 0x3c, 0xfb, 0x8a, 0x24, 0x0b, 0xe2, 0x32, 0xfb, 0xf4, 0xe8, 0x37, 0x55, 0xd5, 0x9d, 0x20, 0xbc, 0x2f, 0xaa, 0x2e, 0xa5, 0xe5, 0xb0, 0x42, 0x84, 0x27, 0x48, 0x5c, 0xca, 0x5e, 0x76, 0xa8, 0x9f, 0xe3, 0x2b, 0xdd, 0x59, 0xab, 0x41, 0x77, 0xad, 0x7c, 0xb1, 0x89, 0x9c, 0x10, 0x1e, 0x3c, 0x4f, 0x75, 0x35, 0x12, 0x95, 0x91, 0x39, 0x0e, 0xbd, 0xf3, 0x01, 0x40, 0x84, 0x60, 0x78, 0xb1, 0x38, 0x67, 0xbb, 0xb2, 0xef, 0xd6, 0xcf, 0x43, 0x4a, 0xfe, 0x35, 0x6e, 0xb1, 0x8d, 0x71, 0x6b, 0x21, 0xfd, 0x66, 0x4c, 0x26, 0xc9, 0x08, 0x49, 0x65, 0x34, 0xbf, 0x2c, 0xde, 0x6d, 0x6b, 0x89, 0x77, 0x99, 0x01, 0x65, 0x94, 0xfb, 0x6d, 0x9f, 0x83, 0x0a, 0xe5, 0xf4, 0x4c, 0xce, 0xc2, 0x6d, 0x42, 0xff, 0x0d, 0x1a, 0x21, 0xb8, 0x0c, 0xdb, 0xe8, 0xc8, 0xc1, 0x70, 0xa5, 0xf7, 0x66, 0xfa, 0xd8, 0x84, 0xab, 0xcc, 0x78, 0x1b, 0x5b, 0x8e, 0xbc, 0x0f, 0x55, 0x9b, 0xfe, 0xaa, 0x45, 0x57, 0xb0, 0x4d, 0x97, 0x7d, 0x51, 0x41, 0x1a, 0x7f, 0x47, 0xbf, 0x43, 0x7d, 0x02, 0x80, 0xcf, 0x9f, 0x92, 0xbc, 0x4f, 0x9c, 0xd6, 0x22, 0x63, 0x37, 0xa4, 0x92, 0x32, 0x08, 0x51, 0x95, 0x5a, 0xda, 0xe2, 0xca, 0xfe, 0xa2, 0x2a, 0x89, 0xc3, 0x13, 0x2d, 0xd2, 0x52, 0xe4, 0x72, 0x83, 0x28, 0xed, 0xa0, 0x55, 0x55, 0xdf, 0xf3, 0x24, 0x14, 0x04, 0x34, 0x1b, 0x8a, 0xa5, 0x02, 0xd4, 0x5c, 0x45, 0x61, 0x13, 0xaf, 0x42, 0xa8, 0xe9, 0x1a, 0x85, 0xe4, 0xb4, 0xe9, 0x55, 0x50, 0x28, 0x98, 0x2e, 0xc3, 0xd1, 0x44, 0x72, 0x2a, 0xf0, 0xeb, 0x04, 0xa6, 0xd3, 0xb8, 0x12, 0x7c, 0x30, 0x40, 0x62, 0x9d, 0xe5, 0x3f, 0x5f, 0xd1, 0x87, 0x04, 0x81, 0x98, 0xe8, 0xf8, 0xe8, 0xcc, 0x85, 0x7a, 0xfc, 0xba, 0xe4, 0x5c, 0x69, 0x3f, 0xec, 0x12, 0xfc, 0x21, 0x49, 0xd5, 0xe7, 0x58, 0x7d, 0x01, 0x21, 0xb1, 0x71, 0x7d, 0x01, 0x47, 0xf6, 0x97, 0x9f, 0x75, 0xe8, 0xf0, 0x85, 0x29, 0x3f, 0x70, 0x5c, 0x33, 0x99, 0xa6, 0xcc, 0x8d, 0xf7, 0x05, 0x7b, 0xf4, 0x81, 0xe6, 0xc3, 0x74, 0xed, 0xf0, 0xa0, 0xaf, 0x74, 0x79, 0xf8, 0x58, 0x04, 0x53, 0x57, 0xb7, 0xfe, 0x21, 0x02, 0x1c, 0x3f, 0xab, 0xda, 0xf0, 0x12, 0x65, 0x2b, 0xf2, 0xe5, 0xdb, 0x25, 0x7b, 0xd9, 0x49, 0x0c, 0xe6, 0x37, 0xa8, 0x14, 0x77, 0xbd, 0x3f, 0x98, 0x14, 0xa2, 0x19, 0x8f, 0xdb, 0x9a, 0xfa, 0x93, 0x44, 0x32, 0x1f, 0x23, 0x93, 0x79, 0x86, 0x70, 0xe5, 0x88, 0xc4, 0x7a, 0x19, 0x24, 0xd5, 0x92, 0xcd, 0xa3, 0xeb, 0x5a, 0x96, 0x75, 0x4d, 0xfd, 0x92, 0xd8, 0x7e, 0xe1, 0xff, 0xa9, 0xd4, 0xee, 0x58, 0x6c, 0x85, 0xd7, 0x51, 0x8c, 0x5d, 0x2d, 0xb5, 0x7d, 0x04, 0x51, 0xc3, 0x3d, 0xe0};
	uint8_t ad[] = {0x32, 0x90, 0xbb, 0x84, 0x41, 0x27, 0x9d, 0xc6, 0x08, 0x3a, 0x43, 0xe9, 0x04, 0x8c, 0x3d, 0xc0, 0x89, 0x66, 0xab, 0x30, 0xd7, 0xa6, 0xb3, 0x57, 0x59, 0xe7, 0xa1, 0x33, 0x39, 0xf1, 0x24, 0x91, 0x8f, 0x3b, 0x5a, 0xb1, 0xaf, 0xfa, 0x65, 0xe6, 0xc0, 0xe3, 0x68, 0x0e, 0xb3, 0x3a, 0x6e, 0xc8, 0x24, 0x24, 0xab, 0x1c, 0xe5, 0xa4, 0x0b, 0x86, 0x54, 0xe1, 0x3d, 0x84, 0x5c, 0x29, 0xb1, 0x38, 0x96, 0xa1, 0x46, 0x6a, 0x75, 0xfc, 0x87, 0x5a, 0xcb, 0xa4, 0x52, 0x7d, 0xed, 0x37, 0xed, 0x00, 0xc6, 0x00, 0xa3, 0x57, 0xc9, 0xa6, 0xe5, 0x86, 0xc7, 0x4c, 0xf3, 0xd8, 0x5c, 0xd3, 0x25, 0x8c, 0x81, 0x32, 0x18, 0xf3, 0x19, 0xd1, 0x2b, 0x82, 0x48, 0x0e, 0x51, 0x24, 0xff, 0x19, 0xec, 0x00, 0xbd, 0xa1, 0xfb, 0xb8, 0xbd, 0x25, 0xee, 0xb3, 0xde, 0x9f, 0xcb, 0xf3, 0x29, 0x6d, 0xeb, 0xa2, 0x50, 0xca, 0xf7, 0xe9, 0xf4, 0xef, 0x0b, 0xe1, 0x91, 0x8e, 0x24, 0x22, 0x1d, 0xd0, 0xbe, 0x88, 0x8c, 0x59, 0xc1, 0x66, 0xad, 0x76, 0x1d, 0x7b, 0x58, 0x46, 0x2a, 0x1b, 0x1d, 0x44, 0xb0, 0x42, 0x65, 0xb4, 0x58, 0x27, 0x17, 0x2c, 0x13, 0x3d, 0xd5, 0xb6, 0xc8, 0x70, 0xb9, 0xaf, 0x7b, 0x21, 0x36, 0x8d, 0x12, 0xa8, 0x8f, 0x4e, 0xfa, 0x17, 0x51, 0x04, 0x75, 0x43, 0xd5, 0x84, 0x38, 0x2d, 0x9e, 0xc2, 0x2e, 0x75, 0x50, 0xd5, 0x0e, 0xcd, 0xdb, 0xa2, 0x7d, 0x1f, 0x65, 0x45, 0x3f, 0x1f, 0x33, 0x98, 0xde, 0x54, 0xee, 0x8c, 0x1f, 0x4a, 0xc8, 0xe1, 0x6f, 0x55, 0x23, 0xd8, 0x96, 0x41, 0xe9, 0x9a, 0x63, 0x23, 0x80, 0xaf, 0x0f, 0x0b, 0x1e, 0x6b, 0x0e, 0x19, 0x2e, 0xc2, 0x9b, 0xf1, 0xd8, 0x71, 0x49, 0x78, 0xff, 0x9f, 0xbf, 0xb9, 0x36, 0x04, 0x14, 0x23, 0x93, 0xe9, 0xa8, 0x2c, 0x3a, 0xae, 0xbb, 0xbe, 0x15, 0xe3, 0xb4, 0xe5, 0xcf, 0xd1, 0x8b, 0xdf, 0xe3, 0x09, 0x31, 0x5c, 0x9f, 0x9f, 0x83, 0x0d, 0xee, 0xbe, 0x2e, 0xdc, 0xdc, 0x24, 0xf8, 0xec, 0xa9, 0x0f, 0xda, 0x49, 0xf6, 0x64, 0x6e, 0x78, 0x9c, 0x50, 0x41, 0xfb, 0x5b, 0xe9, 0x33, 0xfa, 0x84, 0x32, 0x78, 0xe9, 0x5f, 0x3a, 0x54, 0xf8, 0xeb, 0x41, 0xf1, 0x47, 0x77, 0xea, 0x94, 0x9d, 0x5e, 0xa4, 0x42, 0xb0, 0x12, 0x49, 0xe6, 0x48, 0x16, 0x15, 0x1a, 0x32, 0x57, 0x69, 0xe2, 0x64, 0xed, 0x4a, 0xcd, 0x5c, 0x3f, 0x21, 0x70, 0x0c, 0xa7, 0x55, 0xd5, 0xbc, 0x0c, 0x2c, 0x5f, 0x94, 0x53, 0x41, 0x95, 0x10, 0xbc, 0x74, 0xf2, 0xd7, 0x16, 0x21, 0xdc, 0xec, 0xb9, 0xef, 0xc9, 0xc2, 0x47, 0x91, 0xb4, 0xbb, 0x56, 0x0f, 0xb7, 0x0a, 0x82, 0x31, 0x52, 0x1d, 0x65, 0x60, 0xaf, 0x89, 0xd8, 0xd5, 0x01, 0x44, 0xd9, 0xc0, 0x80, 0x86, 0x3f, 0x04, 0x37, 0x81, 0x15, 0x3b, 0xcd, 0x59, 0x03, 0x0e, 0x60, 0xbd, 0x17, 0xa6, 0xd7, 0xaa, 0x08, 0x32, 0x11, 0xb6, 0x7b, 0x58, 0x1f, 0xa4, 0xf7, 0x4c, 0xce, 0x4d, 0x03, 0x0d, 0x1e, 0x8f, 0x94, 0x29, 0xfd, 0x72, 0x5c, 0x11, 0x00, 0x40, 0xd4, 0x1e, 0xb6, 0x98, 0x9f, 0xfb, 0x15, 0x95, 0xc7, 0x2c, 0xbe, 0x3c, 0x9b, 0x78, 0xa8, 0xab, 0x80, 0xd7, 0x1a, 0x6a, 0x52, 0x83, 0xda, 0x77, 0xb8, 0x9c, 0xae, 0x29, 0x5b, 0xb1, 0x3c, 0x14, 0xfb, 0xe4, 0x66, 0xb6, 0x17, 0xf4, 0xda, 0x8a, 0xd6, 0x0b, 0x08, 0x5e, 0x2e, 0xa1, 0x53, 0xf6, 0x71, 0x3a, 0xe0, 0x04, 0x6a, 0xa3, 0x1e, 0x0b, 0xa4, 0x4e, 0x43, 0xef, 0x36, 0xa1, 0x11, 0xbf, 0x05, 0xc0, 0x73, 0xa4, 0xe3, 0x62, 0x4c, 0xd3, 0x5f, 0x63, 0xa5, 0x46, 0xf9, 0x14, 0x2b, 0x35, 0xaa, 0x81, 0xb8, 0x82, 0x6d};
	uint8_t expected[] = {0x88, 0x29, 0x4f, 0xce, 0xf6, 0x5a, 0x1b, 0xdf, 0xd7, 0xba, 0xaa, 0x47, 0x28, 0x16, 0xc6, 0x4e, 0xf5, 0xbe, 0xf2, 0x62, 0x2b, 0x88, 0xc1, 0xec, 0x5a, 0x73, 0x93, 0x96, 0x15, 0x7e, 0xf4, 0x93, 0x5f, 0x3a, 0xa7, 0x64, 0x49, 0xe3, 0x91, 0xc3, 0x2d, 0xa2, 0x8e, 0xe2, 0x85, 0x7f, 0x39, 0x9a, 0xc3, 0xdd, 0x95, 0xae, 0xd3, 0x0c, 0xfb, 0x26, 0xcc, 0x00, 0x63, 0xcd, 0x4c, 0xd8, 0xf7, 0x43, 0x11, 0x08, 0x17, 0x6f, 0xbf, 0x37, 0x01, 0x23, 0x85, 0x66, 0x62, 0xb0, 0x00, 0xa8, 0x34, 0x8e, 0x59, 0x25, 0xfb, 0xb9, 0x7c, 0x9e, 0xc0, 0xc7, 0x37, 0x75, 0x83, 0x30, 0xa7, 0x98, 0x3f, 0x06, 0xb5, 0x15, 0x90, 0xc1, 0xd2, 0xf5, 0xe5, 0xfa, 0xaf, 0x0e, 0xb5, 0x8e, 0x34, 0xe1, 0x9e, 0x5f, 0xc8, 0x5c, 0xec, 0x03, 0xd3, 0x92, 0x6d, 0xd4, 0x6a, 0x79, 0xba, 0x70, 0x26, 0xe8, 0x3d, 0xec, 0x24, 0xe0, 0x74, 0x84, 0xc9, 0x10, 0x3d, 0xd0, 0xcd, 0xb0, 0xed, 0xb5, 0x05, 0x50, 0x0c, 0xac, 0xa5, 0xe1, 0xd5, 0xdb, 0xc7, 0x13, 0x48, 0xcf, 0x00, 0x64, 0x88, 0x21, 0x48, 0x8e, 0xba, 0xab, 0x7f, 0x9d, 0x84, 0xbb, 0xbf, 0x91, 0xb3, 0xc5, 0x21, 0xdb, 0xef, 0x30, 0x11, 0x0e, 0x7b, 0xd9, 0x4f, 0x8d, 0xad, 0x5a, 0xb8, 0xe0, 0xcc, 0x54, 0x11, 0xca, 0x96, 0x82, 0xd2, 0x10, 0xd5, 0xd8, 0x0c, 0x0c, 0x4b, 0xdb, 0xba, 0x81, 0x81, 0x78, 0x9a, 0x42, 0x73, 0xd6, 0xde, 0xb8, 0x08, 0x99, 0xfd, 0xcd, 0x97, 0x6c, 0xa6, 0xf3, 0xa9, 0x77, 0x0b, 0x54, 0x30, 0x5f, 0x58, 0x6a, 0x04, 0x25, 0x6c, 0xfb, 0xeb, 0x4c, 0x11, 0x25, 0x4e, 0x88, 0x55, 0x9f, 0x29, 0x4d, 0xb3, 0xb9, 0xa9, 0x4b, 0x80, 0xab, 0x9f, 0x9a, 0x02, 0xcb, 0x4c, 0x07, 0x48, 0xde, 0x0a, 0xf7, 0x81, 0x86, 0x85, 0x52, 0x16, 0x91, 0xdb, 0xa5, 0x73, 0x8b, 0xe5, 0x46, 0xdb, 0xa1, 0x3a, 0x56, 0x01, 0x6f, 0xb8, 0x63, 0x5a, 0xf9, 0xdf, 0xf5, 0x0f, 0x25, 0xd1, 0xb1, 0x7a, 0xd2, 0x17, 0x07, 0xdb, 0x26, 0x40, 0xa7, 0x6a, 0x74, 0x1e, 0x65, 0xe5, 0x59, 0xb2, 0xaf, 0xaa, 0xec, 0x0f, 0x37, 0xe1, 0x84, 0x36, 0xbf, 0x02, 0x00, 0x8f, 0x84, 0xdb, 0xd7, 0xb2, 0x69, 0x86, 0x87, 0xa2, 0x23, 0x76, 0xb6, 0x5d, 0xc7, 0x52, 0x4f, 0xca, 0x8a, 0x28, 0x70, 0x9e, 0xee, 0x3f, 0x3c, 0xae, 0xe3, 0xb2, 0x8e, 0xd1, 0x17, 0x3d, 0x1e, 0x08, 0xee, 0x84, 0x9e, 0x2c, 0xa6, 0x3d, 0x2c, 0x90, 0xd5, 0x55, 0x75, 0x5c, 0x8f, 0xba, 0xfd, 0x5d, 0x2f, 0x4b, 0x37, 0xf0, 0x6a, 0x1d, 0xbd, 0x68, 0x52, 0xee, 0x2f, 0xfc, 0xfe, 0x79, 0xd5, 0x10, 0x15, 0x2e, 0x98, 0xfc, 0x4f, 0x30, 0x94, 0xf7, 0x40, 0xa4, 0xae, 0xde, 0x9e, 0xe3, 0x78, 0xb6, 0x06, 0xd3, 0x45, 0x76, 0x77, 0x6b, 0xf5, 0xf1, 0x26, 0x9f, 0x53, 0x85, 0xa8, 0x4b, 0x39, 0x28, 0x43, 0x3b, 0xfc, 0xa1, 0x77, 0x55, 0x0c, 0xcf, 0xcd, 0x22, 0xcd, 0x03, 0x31, 0xbb, 0xc5, 0x95, 0xe3, 0x8c, 0x27, 0x58, 0xb2, 0x66, 0x24, 0x76, 0xfa, 0x66, 0x35, 0x4c, 0x4e, 0x84, 0xc7, 0xb3, 0x60, 0x40, 0x5a, 0xa3, 0xf5, 0xb2, 0xa4, 0x86, 0x21, 0xbd, 0xca, 0x1a, 0x90, 0xc6, 0x9b, 0x21, 0x78, 0x9c, 0x91, 0xb5, 0xb8, 0xc5, 0x68, 0xe3, 0xc7, 0x41, 0xd9, 0x9e, 0x22, 0xf6, 0xd7, 0xe2, 0x6f, 0x2a, 0xbe, 0xd0, 0x45, 0xf1, 0xd5, 0x78, 0xb7, 0x82, 0xab, 0x4a, 0x5c, 0xf2, 0xaf, 0x63, 0x6d, 0x84, 0x2b, 0x30, 0x12, 0xe1, 0x80, 0xe4, 0xb0, 0x45, 0xd8, 0xd1, 0x5b, 0x05, 0x7b, 0x69, 0xc9, 0x23, 0x98, 0xa5, 0x17, 0x05, 0x3d, 0xaf, 0x9b, 0xe7, 0xc2, 0x93, 0x5e, 0xa6, 0x16, 0xf0, 0xc2, 0x18, 0xe1, 0x8b, 0x52, 0x6c, 0xf2, 0xa3, 0xf8, 0xc1, 0x15, 0xe2, 0x62};

	uint8_t* crypt = Deoxys_II_encrypt_buffer(key, nonce, message, 512, ad, 513);
	int failed = memcmp(crypt, expected, 528);
	free(crypt);

	printf("Result: %s\n", failed ? "Failed" : "Correct");
	return failed;
}

int main()
{
	test_case_no_message_no_ad();
	test_case_no_message_with_ad();
	test_case_no_message_with_ad2();
	test_case_with_message_no_ad();
	test_case_with_message_no_ad2();
	test_case_with_message_with_ad();
	test_case_with_message_with_ad2();
	test_case_extreme();


	return 0;
}
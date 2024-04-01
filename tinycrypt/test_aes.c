/**
 * @file test_aes.c
 * @author rohirto
 * @brief Demo of AES Encryption using tinycrypt Library
 * @version 0.1
 * @date 2024-04-01
 * 
 * @copyright Copyright (c) 2024
 * 
 * Need to statically link the tinycrpyt library to access aes related functions
 * 
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "lib/include/tinycrypt/aes.h"


#define NUM_OF_NIST_KEYS 16
#define NUM_OF_FIXED_KEYS 128

int main() {
    // Define the plaintext message to encrypt
    uint8_t plaintext[] = "Lorem ipsum";
    size_t plaintext_len = sizeof(plaintext) - 1; // Length of plaintext excluding null terminator

    // Define the AES encryption key (128 bits)
    const uint8_t nist_key[NUM_OF_NIST_KEYS] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};

    struct tc_aes_key_sched_struct s;

    if (tc_aes128_set_encrypt_key(&s, nist_key) == 0)
    {
        //Failed
    }

    uint8_t ciphertext[NUM_OF_NIST_KEYS];
    uint8_t decrypted[NUM_OF_NIST_KEYS];

    (void)tc_aes_encrypt(ciphertext, plaintext, &s);
    tc_aes_decrypt(decrypted, ciphertext, &s);

    printf("%s", decrypted);

    return 0;
}

# Tinycrypt Library

Used in embedded Applications to get access to complex encyption methods like AES, SHA256

## Building the Library
1. Make the Makefile
2. It creates a static Library in lib folder, which needs to statically linked with the application
3. Refer to .h file of the encryption on accessing the functions related to it

Sample Build Command - $ gcc test_aes.c -Llib -ltinycrypt
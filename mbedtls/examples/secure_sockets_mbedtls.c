/**
 * @file secure_sockets_mbedtls.c
 * @author rohirto
 * @brief Create a Secure Sockets connection using mbedtls
 * @version 0.1
 * @date 2024-10-08
 * 
 * @copyright Copyright (c) 2024
 *
 * Build using the Task Build C mbedtls program
 * It will also build files in common folder - Circular buffers
 * Circular Buffers are used to Read with limited footprint
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/x509_crt.h"

#include "common/circular_buffers.h"

#define SERVER_ADDR "docs.github.com"
#define SERVER_PORT "443"

#define CA_CERT_FILE    "/workspaces/docker_c/mbedtls/sample_certs/DigiCert_Global_Root_G2.crt"



int main() {
    int ret;
    mbedtls_net_context server_fd;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    const char *pers = "ssl_client";

    // Initialize the circular buffer
    CircularBuffer cb;
    cb_init(&cb);

    // Initialize structures
    mbedtls_net_init(&server_fd);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_x509_crt_init(&cacert);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    // Seed the random number generator
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0) {
        printf("mbedtls_ctr_drbg_seed failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // Load the CA root certificate
    ret = mbedtls_x509_crt_parse_file(&cacert, CA_CERT_FILE);
    if (ret < 0) {
        printf("mbedtls_x509_crt_parse_file failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // Establish a TCP connection to the server
    if ((ret = mbedtls_net_connect(&server_fd, SERVER_ADDR, SERVER_PORT, MBEDTLS_NET_PROTO_TCP)) != 0) {
        printf("mbedtls_net_connect failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // Set up the SSL configuration
    if ((ret = mbedtls_ssl_config_defaults(&conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        printf("mbedtls_ssl_config_defaults failed, returned -0x%x\n", -ret);
        goto exit;
    }

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED);
    mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);

    // Set up the SSL context
    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
        printf("mbedtls_ssl_setup failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // Set the hostname for Server Name Indication (SNI)
    if ((ret = mbedtls_ssl_set_hostname(&ssl, SERVER_ADDR)) != 0) {
        printf("mbedtls_ssl_set_hostname failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // Set the underlying socket
    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    // Perform the SSL handshake
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            printf("mbedtls_ssl_handshake failed, returned -0x%x\n", -ret);
            goto exit;
        }
    }

    printf("SSL handshake successful.\n");

    // Send data
    const char *http_request = "GET / HTTP/1.1\r\nHost: docs.github.com\r\n\r\n";
    if ((ret = mbedtls_ssl_write(&ssl, (const unsigned char *)http_request, strlen(http_request))) <= 0) {
        printf("mbedtls_ssl_write failed, returned -0x%x\n", -ret);
        goto exit;
    }

    // // Receive data
    // unsigned char buf[8192];
    // memset(buf, 0, sizeof(buf));
    // ret = mbedtls_ssl_read(&ssl, buf, sizeof(buf) - 1);
    // if (ret <= 0) {
    //     printf("mbedtls_ssl_read failed, returned -0x%x\n", -ret);
    //     goto exit;
    // }

    // printf("Received: %s\n", buf);

    // Read data using SSL and store it in the circular buffer
    while (1) {
        int ret = ssl_read_to_circular_buffer(&ssl, &cb);
        if (ret < 0) {
            printf("Failed to read data.\n");
            break;
        }

        // Process data from the circular buffer
        unsigned char data;
        while (!cb_is_empty(&cb)) {
            cb_pop(&cb, &data);
            // Process data (e.g., print it, use it, etc.)
            printf("%c", data);
        }
    }

    // Clean up
exit:
    mbedtls_ssl_close_notify(&ssl);
    mbedtls_net_free(&server_fd);
    mbedtls_x509_crt_free(&cacert);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ret;
}

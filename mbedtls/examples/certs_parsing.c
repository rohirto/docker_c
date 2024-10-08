#include <stdio.h>
#include <stdlib.h>
#include "mbedtls/ssl.h"
#include "mbedtls/error.h"
#include "mbedtls/x509_crt.h"

//#define CA_CERT_FILE    "/workspaces/docker_c/mbedtls/certs/mosquitto-certificate-authority.crt"
#define CA_CERT_FILE    "/workspaces/docker_c/mbedtls/sample_certs/AC_RAIZ_FNMT-RCM.crt"

int main(void)
{
    mbedtls_ssl_context ssl;
    mbedtls_x509_crt ca_cert;
    char buf[1024];  // Buffer to hold certificate info
    int ret;

    // Initialize SSL and X.509 certificate
    mbedtls_ssl_init(&ssl);
    mbedtls_x509_crt_init(&ca_cert);

    // Parse a CA certificate from file (replace with your path)
    ret = mbedtls_x509_crt_parse_file(&ca_cert, CA_CERT_FILE);
    if (ret != 0)
    {
        char error_buf[100];
        mbedtls_strerror(ret, error_buf, sizeof(error_buf));
        printf("Failed to parse certificate: %s\n", error_buf);
        goto exit;
    }

    printf("CA certificate loaded successfully.\n");

     // Print the details of the CA certificate
    mbedtls_x509_crt_info(buf, sizeof(buf) - 1, "", &ca_cert);
    printf("CA Certificate Info:\n%s\n", buf);

exit:
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_ssl_free(&ssl);

    return ret;
}

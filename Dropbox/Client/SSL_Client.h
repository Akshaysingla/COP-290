#include <openssl/ssl.h>
#include <openssl/err.h>

SSL_CTX* InitCTX(void);
void ShowCerts(SSL* ssl);

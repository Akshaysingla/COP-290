#ifndef CLIENT_LOGIN_H
#define CLIENT_LOGIN_H

#include <openssl/ssl.h>

class ClientLogin {
    char* UserName;
    char* Password;
    int client_socket;
    SSL *ssl;
public:
    ClientLogin ( char *usr, char *pass, int sock, SSL *sslt );
    bool Login();
};


#endif // CLIENT_LOGIN_H

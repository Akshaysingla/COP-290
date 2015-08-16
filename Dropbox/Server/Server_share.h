#ifndef SERVER_SHARE_H
#define SERVER_SHARE_H

#include <openssl/ssl.h>
#include <openssl/err.h>


class ServerShare {
public :
    ServerShare( SSL* , char* );
    void setPrivelege ();
    void changePrivelege ();
    void unShare ();
    void showSharedByMe ();
    void showSharedWithMe ();
private:
    SSL *ssl;
    char* username;
};

#endif // SERVER_SHARE_H
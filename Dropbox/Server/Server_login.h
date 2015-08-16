#include "openssl/ssl.h"

class ServerLogin {
	int client_socket;
	SSL *ssl;
public:
	ServerLogin ( int sock, SSL *sslt );
	bool checkExist ( char *usr, char *pass );
	int LoginAccount ( char username[256] );
};

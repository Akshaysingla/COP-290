#include "openssl/ssl.h"

class ServerRegister {
	int client_socket;
	SSL *ssl;
public:
	ServerRegister ( int sock, SSL *sslt );
	bool checkExist ( char *usr );
	void registerAccount ();
};
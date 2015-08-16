#include <openssl/ssl.h>

class ClientRegister {
	char* UserName;
	char* Password;
	char* FirstName;
	char* LastName;
    int client_socket;
    SSL *ssl;
public:
    ClientRegister ( char *usr, char *pass, char* fn, char* ln, int sock, SSL *sslt );
	int checkPassword ( char* ConfirmPass );
    bool registerAccount ();
};

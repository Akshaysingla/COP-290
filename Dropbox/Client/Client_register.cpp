#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <openssl/sha.h>
#include "Client_register.h"

using namespace std;

ClientRegister::ClientRegister ( char *usr, char *pass, char* fn, char* ln, int sock, SSL* sslt ) {
	UserName = usr;
	Password = pass;
	FirstName = fn;
	LastName = ln;
    client_socket = sock;
    ssl = sslt;
}

int ClientRegister::checkPassword ( char* ConfirmPass ) {
	if ( strcmp ( Password, ConfirmPass ) == 0 )
        if ( strlen ( Password ) >= 6 )
            return 0;
        else
            return -2;
	else
		return -1;
}

bool ClientRegister::registerAccount () {
	int status;
	do {
        status = SSL_write ( ssl, UserName, 256 );
	} while ( status < 0 );
	int exist;
	do {
        status = SSL_read ( ssl, &exist, 4 );
	} while ( status < 0 );
    if ( exist == 0 )
		return 0;
	do {
        status = SSL_write ( ssl, Password, 256 );
	} while ( status < 0 );
	do {
        status = SSL_write ( ssl, FirstName, 256 );
	} while ( status < 0 );
	do {
        status = SSL_write ( ssl, LastName, 256 );
	} while ( status < 0 );
	return 1;
}


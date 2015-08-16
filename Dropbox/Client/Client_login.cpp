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
#include "Client_login.h"

using namespace std;

ClientLogin::ClientLogin ( char *usr, char *pass, int sock, SSL *sslt ) {
    UserName = usr;
    Password = pass;
    client_socket = sock;
    ssl = sslt;
}


bool ClientLogin::Login() {
    int status;
    do {
        status = SSL_write ( ssl, UserName, 256 );
    } while ( status < 0 );
    int check;
    do {
        status = SSL_write ( ssl, Password, 256 );
    } while ( status < 0 );
    do {
        status = SSL_read ( ssl, &check, 4 );
    } while ( status < 0 );
    return check;
}

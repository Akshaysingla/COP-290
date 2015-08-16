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
#include "Client_dropbox.h"

using namespace std;

Client_dropbox::Client_dropbox(char *Host_Name, int portno){
    HostName = Host_Name;
    port_no = portno;
    sockettoserver = 0;
}

int Client_dropbox::CreateSocketToServer(){
    sockaddr_in server;
    hostent *host;
    host = gethostbyname(HostName);
    client_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (client_socket == -1){
        cout << "Could not create client_socket";
        sockettoserver = 0;
    }
    memset ( &server, 0, sizeof(server) );
    server.sin_family = AF_INET;
    bcopy ( (char *)host->h_addr, (char *)&server.sin_addr.s_addr, host->h_length );
    server.sin_port = htons( port_no );
    if ( connect (client_socket , (sockaddr*)&server , sizeof(server)) < 0 ) {
        cout << "Error Connecting\n";
        sockettoserver = 0 ;
        return -1;
    }
    cout << "Created Socket\n";
    sockettoserver = 1;
    return client_socket;
}

bool Client_dropbox::socket_to_server(){
    return sockettoserver;
}

void Client_dropbox::deleteSocket(){
	int status;
    int option = 3;
    do {
        status = write ( client_socket, (void*)&option, 4 );
    } while ( status < 0 );
    close ( client_socket);
}

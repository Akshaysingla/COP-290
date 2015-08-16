#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>   
#include <unistd.h>  
#include <iostream>
#include <fstream>
#include "Server_dropbox.h"

using namespace std;

int main ( int argc, char** argv ) {
    Server_dropbox temp;
    temp.establish_server( atoi(argv[1]) );
    return 0;
}
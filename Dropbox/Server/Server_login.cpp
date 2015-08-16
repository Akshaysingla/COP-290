#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "SQL/include/sqlite3.h"
#include "Server_login.h"

using namespace std;

ServerLogin::ServerLogin ( int sock, SSL *sslt ) {
	client_socket = sock;
	ssl = sslt;
}


static int callback(void *data, int argc, char **argv, char **azColName) {
	static int num = 0;
	num = (long)data;
	num++;
	*static_cast<int*>(data) = num;
	return 0;
}

bool ServerLogin::checkExist ( char *usr, char *pass ) {
	int send;
	int num;
	char sql[255];
	char *zErrMsg = 0;
	sqlite3 *db;
	sqlite3_open("MyDropbox.db", &db);
	strcpy(sql, "SELECT * from USERS where USERNAME=\'");
	strcat ( sql, usr );
	strcat ( sql, "\' AND PASSWORD=\'" );
	strcat ( sql, pass );
	strcat ( sql, "\'" );
	num = 0;
	sqlite3_exec(db, sql, callback, &num, &zErrMsg);
	sqlite3_close(db);
	return ( num != 0 );
				
}

int ServerLogin::LoginAccount( char username[256] ) {
	fstream f;
	int exist;
	int status;
	int login;
	char filename[256];
	char readuser[256], readpass[256], Filereaduser[256], Filereadpass[256];
	do {
		do {
				status = SSL_read ( ssl, &readuser, 256 );
		} while ( status < 0 );
		if ( strcmp ( readuser, "" ) == 0 )
			return 0;
		do {
				status = SSL_read ( ssl, &readpass, 256 );
		} while ( status < 0 );
		if ( strcmp ( readpass, "" ) == 0 )
			return 0;
		login = checkExist ( readuser, readpass );
		do {
				status = SSL_write ( ssl, &login, 4 );
		} while ( status < 0 );
		if ( login )
			strcpy ( username, readuser );
    } while ( login == 0 );
}

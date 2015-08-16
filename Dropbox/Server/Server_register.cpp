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
#include "Server_register.h"

using namespace std;

int val = 0;

ServerRegister::ServerRegister ( int sock, SSL *sslt ) {
	client_socket = sock;
	ssl = sslt;
}

static int callback(void *data, int argc, char **argv, char **azColName) {
	val++;
	return 0;
}

bool ServerRegister::checkExist ( char *usr ) {
	int send;
	char sql[255];
	char *zErrMsg = 0;
	sqlite3 *db;
	sqlite3_open("MyDropbox.db", &db);
	strcpy(sql, "SELECT * from USERS where USERNAME=\'");
	strcat ( sql, usr );
	strcat ( sql, "\'" );
	sqlite3_exec(db, sql, callback, (void*)send, &zErrMsg);
	sqlite3_close(db);
	return ( val == 0 );
}

void ServerRegister::registerAccount () {
	char username[256], password[256], firstname[256], lastname[256];
	int status;
	int exist;
	char filename[255];
	do {
		do {
			status = SSL_read ( ssl, &username, 256 );
		} while ( status < 0 );
		if ( strcmp ( username, "" ) == 0 )
			return;
		val = 0;
		exist = checkExist ( username );
		if ( exist > 0 ) exist = 1;
		do {
				status = SSL_write ( ssl, &exist, 4 );
		} while ( status < 0 );
	} while ( exist == 0 );
	char foldername[255];
	//strcpy ( foldername, "" );
	//strcat ( foldername, "mkdir ");
	//strcat( foldername, "Files//" );
	//strcat ( foldername, username );
	//const char *Foldername = foldername;
	//system ( Foldername );
	do {
		status = SSL_read ( ssl, &password, 256 );
	} while ( status < 0 );
	do {
		status = SSL_read ( ssl, &firstname, 256 );
	} while ( status < 0 );
	do {
		status = SSL_read ( ssl, &lastname, 256 );
	} while ( status < 0 );
	char sql[255];
	sqlite3 *db;
	char *zErrMsg = 0;
	sqlite3_open("MyDropbox.db", &db);
	strcpy ( sql, "INSERT INTO USERS (USERNAME, PASSWORD, FIRSTNAME, LASTNAME) VALUES ( ");
	strcat ( sql, "\'" );
	strcat ( sql, username );
	strcat ( sql, "\'" );
	strcat ( sql, ", " );
	strcat ( sql, "\'" );
	strcat ( sql, password );
	strcat ( sql, "\'" );
	strcat ( sql, ", " );
	strcat ( sql, "\'" );
	strcat ( sql, firstname );
	strcat ( sql, "\'" );
	strcat ( sql, ", " );
	strcat ( sql, "\'" );
	strcat ( sql,  lastname );
	strcat ( sql, "\'" );
	strcat ( sql, ");" );
	sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	sqlite3_close(db);
}
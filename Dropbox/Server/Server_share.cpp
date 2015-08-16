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
#include "Server_share.h"

char share_with[255];
int priv_type;

static int callback1(void *data, int argc, char **argv, char **azColName) {
    sqlite3 *same_db = (sqlite3*)data;
    char sql[255];
    char *zErrMsg = 0;
    strcpy ( sql, "INSERT INTO DROPBOX VALUES ( ");
    strcat ( sql, "\'" );
    strcat ( sql, share_with );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, argv[1] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, "/Shared/" );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, argv[3] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[4] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    if ( priv_type == 0 )
    	strcat ( sql,  "10" );
    else
    	strcat ( sql,  "11" );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[6] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[0] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[8] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[9] );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  argv[10] );
    strcat ( sql, "\'" );
    strcat ( sql, " );" );
    std::cout<<sql<<std::endl;
    sqlite3_exec(same_db, sql, NULL, 0, &zErrMsg);

    /*char new_shared_with[255];
    if ( strcmp ( argv[7], "NULL") == 0 )
    	strcpy ( new_shared_with, share_with );
    else {
    	strcpy ( new_shared_with, argv[7] );
    	strcat ( new_shared_with, " " );
    	strcat ( new_shared_with, share_with );
    }
    strcpy ( sql, "UPDATE DROPBOX SET SHARED_WITH = ");
    strcat ( sql, "\'" );
    strcat ( sql, new_shared_with );
    strcat ( sql, "\'" );
    strcat ( sql, " where USERNAME = ");
    strcat ( sql, "\'");
    strcat ( sql, argv[0] );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ")
    strcat ( sql, "\'");
    strcat ( sql, argv[1] );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ")
    strcat ( sql, "\'");
    strcat ( sql, argv[2] );
    strcat ( sql, "\'" );
    sqlite3_exec(same_db, sql, NULL, 0, &zErrMsg);*/
}

static int callback2 (void *data, int argc, char **argv, char **azColName) {
    SSL* ssl = (SSL*)data;
    int status;
    do {
        status = SSL_write ( ssl, argv[0], 256 );
    } while ( status < 0 );
    do {
        status = SSL_write ( ssl, argv[1], 256 );
    } while ( status < 0 );
    int priv;
    if  ( strcmp ( argv[2], "10") == 0 )
        priv = 0;
    else
        priv = 1;
    do {
        status = SSL_write ( ssl, &priv, 4 );
    } while ( status < 0 );
    return 0;
}

ServerShare::ServerShare ( SSL *sslt, char* user ) {
	ssl = sslt;
    username = user;
}

void ServerShare::setPrivelege (  ) {
    int status;
    char file[255], path[255], sharee[255];
    int priv;
    std::cout<<"Privlege"<<std::endl;
    do {
        status = SSL_read ( ssl, &file, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<file<<std::endl;
    do {
        status = SSL_read ( ssl, &path, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<path<<std::endl;
    do {
        status = SSL_read ( ssl, &sharee, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<sharee<<std::endl;
    do {
        status = SSL_read ( ssl, &priv, 4 );
    } while ( status < 0 );
	std::cout<<"Privlege : "<<priv<<std::endl;
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    strcpy ( share_with, sharee );
    priv_type = priv;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "SELECT * from DROPBOX where USERNAME = " );
    strcat ( sql, "\'");
    strcat ( sql, username );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ");
    strcat ( sql, "\'");
    strcat ( sql, file );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'");
    strcat ( sql, path );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    std::cout<<sql<<std::endl;
    sqlite3_exec(db, sql, callback1, (void*)db, &zErrMsg);
    if ( priv == 1 ) {
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS + 1 where FILEID = (SELECT FILEID from DROPBOX where USERNAME = " );
        strcat ( sql, "\'");
        strcat ( sql, username );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'");
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'");
        strcat ( sql, path );
        strcat ( sql, "\'" );
        strcat ( sql, ")" );
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
    sqlite3_close ( db );
}

void ServerShare::changePrivelege () {
    int status;
    char file[255], sharee[255];
    int priv;
    std::cout<<"Privlege change reached"<<std::endl;
    do {
        status = SSL_read ( ssl, &file, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<file<<std::endl;
    do {
        status = SSL_read ( ssl, &sharee, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<sharee<<std::endl;
    do {
        status = SSL_read ( ssl, &priv, 4 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<priv<<std::endl;
	char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "UPDATE DROPBOX SET DELETED = " );
    strcat ( sql, "\'" );
    if ( priv == 0 )
    	strcat ( sql,  "10" );
    else
    	strcat ( sql,  "11" );
    strcat ( sql, "\'" );
    strcat ( sql, " where USERNAME = ");
    strcat ( sql, "\'");
    strcat ( sql, sharee );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ");
    strcat ( sql, "\'");
    strcat ( sql, file );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'");
    strcat ( sql, "/Shared/" );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if ( priv == 0 ) {
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS - 1 where FILEID = (SELECT FILEID from DROPBOX where USERNAME = " );
        strcat ( sql, "\'");
        strcat ( sql, sharee );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'");
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'");
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        strcat ( sql, ")" );    
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
    else {
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS + 1 where FILEID = (SELECT FILEID from DROPBOX where USERNAME = " );
        strcat ( sql, "\'");
        strcat ( sql, sharee );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'");
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'");
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        strcat ( sql, ")" );    
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
    sqlite3_close ( db );
}

void ServerShare::unShare () {
    int status;
    char file[255], sharee[255];
    int priv;
    std::cout<<"Privlege"<<std::endl;
    do {
        status = SSL_read ( ssl, &file, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<file<<std::endl;
    do {
        status = SSL_read ( ssl, &sharee, 256 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<sharee<<std::endl;
    do {
        status = SSL_read ( ssl, &priv, 4 );
    } while ( status < 0 );
    std::cout<<"Privlege : "<<priv<<std::endl;
	char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    if ( priv == 1 ) {
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS - 1 where FILEID = (SELECT FILEID from DROPBOX where USERNAME = " );
        strcat ( sql, "\'");
        strcat ( sql, sharee );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'");
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'");
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        strcat ( sql, ")" );    
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
    strcpy ( sql, "DELETE FROM DROPBOX where USERNAME =  " );
    strcat ( sql, "\'");
    strcat ( sql, sharee );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ");
    strcat ( sql, "\'");
    strcat ( sql, file );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'");
    strcat ( sql, "/Shared/" );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    sqlite3_close ( db );
}

void ServerShare::showSharedByMe ( ) {
    char sql[255];
    int status;
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "SELECT FILENAME, USERNAME, DELETED from DROPBOX where SHARED_FROM =  " );
    strcat ( sql, "\'");
    strcat ( sql, username );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    sqlite3_exec(db, sql, callback2, (void*)ssl, &zErrMsg);
    std::cout << sql << std::endl;
    char* last = "";
    do {
        status = SSL_write ( ssl, last, 256 );
    } while ( status < 0 );
    sqlite3_close ( db );
}

void ServerShare::showSharedWithMe ( ) {
	char sql[255];
    int status;
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "SELECT FILENAME, SHARED_FROM, DELETED from DROPBOX where USERNAME =  " );
    strcat ( sql, "\'");
    strcat ( sql, username );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'");
    strcat ( sql, "/Shared/" );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    sqlite3_exec(db, sql, callback2, (void*)ssl, &zErrMsg);
    std::cout << sql << std::endl;
    char* last = "";
    do {
        status = SSL_write ( ssl, last, 256 );
    } while ( status < 0 );
    sqlite3_close ( db );
}



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
#include "Server_dropbox.h"
#include "Server_sync.h"

using namespace std;

Server_dropbox *usenow;
SSL *ssluse;
bool update;
int found;
char lastedited[255];
int deleted;
char FileIdExt[255];

static int callback(void *data, int argc, char **argv, char **azColName) {
	Server_dropbox *temp = (Server_dropbox*)data;
	char FileIdExt[256];
	strcpy ( FileIdExt, argv[1] );
	strcat ( FileIdExt, argv[2] );
	temp->SendFile( argv[0] , FileIdExt, argv[3], argv[4] );
}

static int callback1(void *data, int argc, char **argv, char **azColName) {
	char *temp = (char*)data;
	found=1;
	char DatemodClient[256], DatemodServer[256];
	bzero ( DatemodServer, sizeof ( DatemodServer ));
	strcpy ( DatemodClient, temp );
	strcat ( DatemodServer, argv[0] );
	std::cout<<"flag00 "<<std::endl<<DatemodServer<<" "<<DatemodClient<<std::endl;

	int status;
	if( strcmp (DatemodClient, DatemodServer) <= 0) {
		update = false;
		std::cout<<"flag01 "<<update<<std::endl;
	}
	else {
		update = true;
		std::cout<<"flag01 "<<update<<std::endl;
	}
}

static int callback2(void *data, int argc, char **argv, char **azColName) {
	char sys[255];
	strcpy ( sys, "rm -f Files/");
	strcat ( sys, argv[0] );
	strcat ( sys, argv[1] ); 
	system ( sys );
}

static int callback3(void *data, int argc, char **argv, char **azColName) {
	SSL *ssl = (SSL*)data;
	int status;
	do {
		status = SSL_write ( ssl, argv[0], 256 );
	} while ( status < 0 );
}

static int callback4(void *data, int argc, char **argv, char **azColName) {
	cout<<"callback4"<<argv[0]<<"|"<<argv[1]<<"|"<<argv[2]<<"|"<<argv[3]<<endl;
	strcpy ( lastedited , argv [0] );
	deleted = atoi(argv[1]);
	char id[255];
	strcpy ( FileIdExt, argv[2] );
	strcat ( FileIdExt, argv[3] );
	found=1;
	return 0;
}

ServerSync::ServerSync ( Server_dropbox *Serverdropbox, SSL* sslt ) {
	Dropbox = Serverdropbox;
	ssl = sslt;
	usenow = Dropbox;
	ssluse = sslt;
}

void ServerSync::AllFilesFromServer (char* username) {
	int status;
	char sql[255];
	sqlite3 *db;
	char *zErrMsg = 0;
	sqlite3_open("MyDropbox.db", &db);
	strcpy ( sql, "SELECT FILENAME, FILEID, EXTENSION, FILEPATH , LAST_EDITED from DROPBOX where DELETED = 1 AND USERNAME =" );
	strcat ( sql, "\'" );
	strcat ( sql, username);
	strcat ( sql, "\'");
	strcat ( sql, ";");
	sqlite3_exec(db, sql, callback, (void*)Dropbox, &zErrMsg);
	sqlite3_close ( db	);
	char cancel[255];
	strcpy ( cancel , "");
	do {
		status = SSL_write ( ssl, cancel, 256 );
	} while ( status < 0 );
	return ;
}


void ServerSync::SyncAll(char* username) {

	usenow = Dropbox;
	int status;
	int option;
	do {
		status = SSL_read ( ssl, &option, 4 );
	} while ( status < 0 );
	std::cout<<"option "<<option<<std::endl;
	
	if ( option == 0 ) {
		AllFilesFromServer(username);
		std::cout<<"Last sync waiting"<<std::endl;
		char lastsync[255];
		do {
			status = SSL_read ( ssl, &lastsync, 256 );
		} while ( status < 0 );
		std::cout<<"Last sync "<<lastsync<<std::endl;
		char sql[255];
		sqlite3 *db;
		char *zErrMsg = 0;
		sqlite3_open("MyDropbox.db", &db);
		strcpy ( sql , "UPDATE USERS SET LASTSYNC = ");
		strcat ( sql, "\'" );
		strcat ( sql, lastsync);
		strcat ( sql, "\'");
		strcat ( sql , " where USERNAME =  " );
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		sqlite3_close ( db	);
	}
	else if (option == 1) {
		char sql[255];
		sqlite3 *db;
		char *zErrMsg = 0;
		::found = 0;
		sqlite3_open("MyDropbox.db", &db);
		strcpy ( sql, "SELECT LASTSYNC FROM USERS where USERNAME = " );
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		sqlite3_exec(db, sql, callback3, (void*)ssl, &zErrMsg);
		int ClientTimeCheck;
		do {
			status = SSL_read ( ssl, &ClientTimeCheck, 4 );
		} while ( status < 0 );
		if(ClientTimeCheck == 0) {	
			//std::cout<<"option1"<<std::endl;
			AllFilesFromClient(username);
			strcpy ( sql, "SELECT FILEID, EXTENSION, USERNAME FROM DROPBOX where DELETED = 1 AND OWNERS = 1 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, callback2, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 3 WHERE FILEID IN (SELECT FILEID FROM DROPBOX where DELETED = 1 AND OWNERS = 1 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ")");
			cout<<"Query 1 : "<<sql<<endl;
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS - 1 where FILEID IN (SELECT FILEID FROM DROPBOX WHERE DELETED = 1 AND OWNERS <> 1 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ")");
			cout<<"Query 2 : "<<sql<<endl;
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 3 where DELETED = 1 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 1 where DELETED = 2 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 10 where DELETED = 12 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 11 where DELETED = 13 AND USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			sqlite3_close ( db	);
		}
		else if ( ClientTimeCheck == 1 ){
			std::cout<<"Different time sync"<<std::endl;
			SyncClientServer(username);
		}
		std::cout<<"Last sync waiting"<<std::endl;
		char lastsync[255];
		do {
		status = SSL_read ( ssl, &lastsync, 256 );
		} while ( status < 0 );
		std::cout<<"Last sync "<<lastsync<<std::endl;
		sqlite3_open("MyDropbox.db", &db);
		strcpy ( sql , "UPDATE USERS SET LASTSYNC = ");
		strcat ( sql, "\'" );
		strcat ( sql, lastsync);
		strcat ( sql, "\'");
		strcat ( sql , " where USERNAME =  " );
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		strcpy ( sql, "UPDATE DROPBOX SET LAST_EDITED = ");
		strcat ( sql, "\'" );
		strcat ( sql, lastsync);
		strcat ( sql, "\'");
		strcat ( sql , " where DELETED = 3 AND USERNAME =  " );
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		strcpy ( sql, "UPDATE DROPBOX SET DELETED = 0 where DELETED = 3; ");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		sqlite3_close ( db	);
	}
}

void ServerSync::AllFilesFromClient(char* username) {
	char filename[255];
	char filepath[255];
	char datemod[255];
	found = 0;
	int status;
	while (1) {
		do {
			status = SSL_read ( ssl, &filename, 256 );
		} while ( status < 0 );
		//std::cout<<"readfilename"<<filename<<std::endl;
		if(strcmp (filename, "") == 0) {
			break;
		}
	
		do {
			status = SSL_read ( ssl, &filepath, 256 );
		} while ( status < 0 );
		//std::cout<<"readfilepath "<<filepath<<std::endl;

				do {
			status = SSL_read ( ssl, &datemod, 256 );
		} while ( status < 0 );
		//std::cout<<"datemod "<<datemod<<std::endl;
		char sql[255];
		sqlite3 *db;
		char *zErrMsg = 0;
		::found = 0;
		sqlite3_open("MyDropbox.db", &db);
		strcpy ( sql, "SELECT LAST_EDITED from DROPBOX where USERNAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILENAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, filename);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILEPATH = ");
		strcat ( sql, "\'" );
		strcat ( sql, filepath);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, callback1, (void*)datemod, &zErrMsg);
		std::cout<<"Reached here now "<< ::found<<std::endl;
		if (!::found) {
			int receive;
			bzero ( &receive, sizeof ( &receive ));
			receive = 15;
			do {
				status = SSL_write ( ssl, &receive, 4 );
			} while ( status < 0 );
			std::cout<<"Calling Store File "<<std::endl;
			Dropbox->StoreFile();
		}
		else {
			if (::update) {
				int receive;;
				bzero ( &receive, sizeof ( &receive ));
				receive = 15;
				do {
					status = SSL_write ( ssl, &receive, 4 );
				} while ( status < 0 );
				std::cout<<"Calling Update Store File "<<std::endl;
				Dropbox->UpdateStoreFile();
			}
			else {
				int cancel = -1 ;
				
				do {
					status = SSL_write ( ssl, &cancel, 4 );
				} while ( status < 0 );
			}


		}
		strcpy ( sql, "UPDATE DROPBOX SET DELETED = 2 where DELETED = 1 AND USERNAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILENAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, filename);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILEPATH = ");
		strcat ( sql, "\'" );
		strcat ( sql, filepath);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		strcpy ( sql, "UPDATE DROPBOX SET DELETED = 10 where DELETED = 12 AND USERNAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILENAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, filename);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILEPATH = ");
		strcat ( sql, "\'" );
		strcat ( sql, filepath);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		strcpy ( sql, "UPDATE DROPBOX SET DELETED = 11 where DELETED = 13 AND USERNAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILENAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, filename);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILEPATH = ");
		strcat ( sql, "\'" );
		strcat ( sql, filepath);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
		sqlite3_close ( db	);
	}
}

void ServerSync::SyncClientServer(char* username){
	char filename[255];
	char filepath[255];
	char datemod[255];
	int status;
	while (1) {
		do {
			status = SSL_read ( ssl, &filename, 256 );
		} while ( status < 0 );
		if(strcmp (filename, "") == 0) {
			break;
		}
		cout<<"Read 1"<<filename<<endl;
		do {
			status = SSL_read ( ssl, &filepath, 256 );
		} while ( status < 0 );
		cout<<"Read 2"<<filepath<<endl;
				do {
			status = SSL_read ( ssl, &datemod, 256 );
		} while ( status < 0 );
		cout<<"Read 3"<<datemod<<endl;
		char sql[255];
		sqlite3 *db;
		char *zErrMsg = 0;
		::found = 0;
		sqlite3_open("MyDropbox.db", &db);
		strcpy ( sql, "SELECT LAST_EDITED, DELETED, FILEID, EXTENSION from DROPBOX where USERNAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, username);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILENAME = ");
		strcat ( sql, "\'" );
		strcat ( sql, filename);
		strcat ( sql, "\'");
		strcat ( sql, " AND FILEPATH = ");
		strcat ( sql, "\'" );
		strcat ( sql, filepath);
		strcat ( sql, "\'");
		strcat ( sql, ";");
		sqlite3_exec(db, sql, callback4, 0, &zErrMsg);
		cout<<"FileData : "<<::FileIdExt<<"|"<<::lastedited<<"|"<<::deleted<<endl;
		if( !found ) {
			int receive;
			bzero ( &receive, sizeof ( &receive ));
			receive = 15;
			do {
				status = SSL_write ( ssl, &receive, 4 );
			} while ( status < 0 );
			std::cout<<"Not on server, Asing client to send "<<std::endl;
			Dropbox->StoreFile();
			char sql[255];
			sqlite3 *db;
			char *zErrMsg = 0;
			::found = 0;
			sqlite3_open("MyDropbox.db", &db);
			strcpy ( sql, "UPDATE DROPBOX SET DELETED = 2 where USERNAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, username);
			strcat ( sql, "\'");
			strcat ( sql, " AND FILENAME = ");
			strcat ( sql, "\'" );
			strcat ( sql, filename);
			strcat ( sql, "\'");
			strcat ( sql, " AND FILEPATH = ");
			strcat ( sql, "\'" );
			strcat ( sql, filepath);
			strcat ( sql, "\'");
			strcat ( sql, ";");
			sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
			sqlite3_close ( db	);
		}
		else {
			int receive;
			if(::deleted == 0)
				if (strcmp (::lastedited, datemod) > 0 ) {
					receive = 17;		//Tell client to delete
					do {
						status = SSL_write ( ssl, &receive, 4 );
					} while ( status < 0 );
					std::cout<<"Asking client to delete "<<std::endl;
				}
				else {
					receive = 15;		//Tell client to send
					do {
						status = SSL_write ( ssl, &receive, 4 );
					} while ( status < 0 );
					Dropbox->StoreFile();
					sqlite3_open("MyDropbox.db", &db);
					strcpy ( sql, "UPDATE DROPBOX SET DELETED = 2 where USERNAME = ");
					strcat ( sql, "\'" );
					strcat ( sql, username);
					strcat ( sql, "\'");
					strcat ( sql, " AND FILENAME = ");
					strcat ( sql, "\'" );
					strcat ( sql, filename);
					strcat ( sql, "\'");
					strcat ( sql, " AND FILEPATH = ");
					strcat ( sql, "\'" );
					strcat ( sql, filepath);
					strcat ( sql, "\'");
					strcat ( sql, ";");
					sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
					sqlite3_close ( db	);
					std::cout<<"Asking client to send, he has more updated file "<<std::endl;
				}
			else {
				if (strcmp (::lastedited, datemod) > 0 ) {
					receive = 16;		//Tell client to receive
					do {
						status = SSL_write ( ssl, &receive, 4 );
					} while ( status < 0 );
					char* Sendfname = filename;
					char* EditHis = ::lastedited;
					char* Sendfpath = filepath; 
					Dropbox->SendFile(Sendfname, ::FileIdExt, Sendfpath, EditHis);
					std::cout<<"Client should take file from me "<<std::endl;
				}
				else if (strcmp (::lastedited, datemod) < 0 ) {
					receive = 15;		//Tell client to send
					do {
						status = SSL_write ( ssl, &receive, 4 );
					} while ( status < 0 );
					Dropbox->UpdateStoreFile();
					std::cout<<"Update existing file from client "<<std::endl;
				}
				else {
					int cancel = -1;
					//bzero ( cancel, sizeof (cancel));
					//strcpy ( cancel , "");
					do {
						status = SSL_write ( ssl, &cancel, 4 );
					} while ( status < 0 );
					std::cout<<"Cancel "<<std::endl;
					sqlite3_open("MyDropbox.db", &db);
					strcpy ( sql, "UPDATE DROPBOX SET DELETED = 2 where USERNAME = ");
					strcat ( sql, "\'" );
					strcat ( sql, username);
					strcat ( sql, "\'");
					strcat ( sql, " AND FILENAME = ");
					strcat ( sql, "\'" );
					strcat ( sql, filename);
					strcat ( sql, "\'");
					strcat ( sql, " AND FILEPATH = ");
					strcat ( sql, "\'" );
					strcat ( sql, filepath);
					strcat ( sql, "\'");
					strcat ( sql, ";");
					sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
					sqlite3_close ( db	);
				}
			}	
		}
	}
	char sql[255];
	sqlite3 *db;
	char *zErrMsg = 0;
	sqlite3_open("MyDropbox.db", &db);
	AllFilesFromServer(username);
	strcpy ( sql, "UPDATE DROPBOX SET DELETED = 1 where DELETED = 2 AND USERNAME = ");
	strcat ( sql, "\'" );
	strcat ( sql, username);
	strcat ( sql, "\'");
	strcat ( sql, ";");
	sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	sqlite3_close ( db	);
}
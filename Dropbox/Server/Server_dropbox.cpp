#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "SQL/include/sqlite3.h"
#include "Server_dropbox.h"
#include "Server_register.h"
#include "Server_login.h"
#include "Server_SSL.h"
#include "Server_sync.h"
#include "Server_share.h"

using namespace std;

Server_dropbox::Server_dropbox(){
    server_connect = 0;
    logged_in = 0;
}

static int callback0(void *data, int argc, char **argv, char **azColName) {
    SSL* ssl = (SSL*)data;
    int status;
    do {
        status = SSL_write ( ssl, argv[0], 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"sending : "<<argv[0]<<endl;
    #endif
    return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName) {
    *static_cast<char**>(data) = argv[0];
    return 0;
}

static int callback2(void *data, int argc, char **argv, char **azColName) {
    SSL* ssl = (SSL*)data;
    int status;
    do {
        status = SSL_write ( ssl, argv[0], 256 );
    } while ( status < 0 );
    do {
        status = SSL_write ( ssl, argv[1], 256 );
    } while ( status < 0 );
    return 0;
}

static int callback3(void *data, int argc, char **argv, char **azColName) {
    SSL* ssl = (SSL*)data;
    int status;
    char temp[255];
    strcpy ( temp, argv[1] );
	char temp2[255];
    strcat ( temp, argv[0] );
    do {
        status = SSL_write ( ssl, temp, 256 );
    } while ( status < 0 );
	
    return 0;
}

static int callback4(void *data, int argc, char **argv, char **azColName) {
    *static_cast<char**>(data) = argv[0];
    return 0;
}

static int callback5(void *data, int argc, char **argv, char **azColName) {
    char sys[255];
    strcpy ( sys, "rm -f Files/");
    strcat ( sys, argv[0] );
    strcat ( sys, argv[1] ); 
    system ( sys );
}


void Server_dropbox::CreateServerSocket( int port ){
    SSL_library_init();
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, "mycert.pem", "mycert.pem"); /* load certs */
    ServerSocket = OpenListener( port );    /* create server socket */
    server_connect = 1;
}

void Server_dropbox::Server_client_connection(){
    while (1) {
        
        socklen_t len = sizeof(Client);
        ClientSocket = accept(ServerSocket, (struct sockaddr*)&Client, &len);  /* accept connection as usual */
        #ifdef DEBUG_SYNC
            printf("Connection: %s:%d\n",inet_ntoa(Client.sin_addr), ntohs(Client.sin_port));
        #endif
        ssl = SSL_new(ctx);              /* get new SSL state with context */
        SSL_set_fd(ssl, ClientSocket);      /* set connection socket to SSL state */
        if ( SSL_accept(ssl) == -1 )     /* do SSL-protocol accept */
            ERR_print_errors_fp(stderr);
        if (fork() == 0) {
            close(ServerSocket);
            do {
                login_register_screen ();
            } while ( !logged_in );
            #ifdef DEBUG_SYNC
                cout<<"Calling Client Request"<<endl;
            #endif
            client_request();
        }
        close(ClientSocket);
    }
}

void Server_dropbox::sync_initial_data() {
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "SELECT FIRSTNAME, LASTNAME from USERS where USERNAME=" );
    strcat ( sql, "\'");
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    sqlite3_exec(db, sql, callback2, (void*)ssl, &zErrMsg);
    strcpy ( sql, "SELECT USERNAME from USERS where USERNAME <> " );
    strcat ( sql, "\'");
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    sqlite3_exec(db, sql, callback0, (void*)ssl, &zErrMsg);
    sqlite3_close ( db );
    char* last = "";
    int status;
    do {
        status = SSL_write ( ssl, last, 256 );
    } while ( status < 0 );
    sqlite3_close ( db );
    send_files();
    ServerShare *share;
    share = new ServerShare(ssl, UserName);
    share->showSharedByMe();
    share->showSharedWithMe();


}

void Server_dropbox::send_files () {
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql, "SELECT FILENAME, FILEPATH from DROPBOX where DELETED = 1 AND USERNAME=" );
    strcat ( sql, "\'");
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    sqlite3_exec(db, sql, callback2, (void*)ssl, &zErrMsg);
    sqlite3_close ( db );
    char* last = "";
    int status;
    do {
        status = SSL_write ( ssl, last, 256 );
    } while ( status < 0 );
    sqlite3_close ( db );
}

void Server_dropbox::client_request(){
    int option;
    int status;
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    ServerSync *sync;
    sync = new ServerSync ( this, ssl );
    sync->SyncAll ( UserName );
    ServerShare *share;
    share = new ServerShare(ssl, UserName);
    //share->setPrivelege();
    //share->unShare();
    sync_initial_data();
    while(1){
        option = 0;
        do{
            status = SSL_read (ssl, &option, 4 );
        } while ( status < 0 );
        switch ( option ) {
            case 20: sync->SyncAll ( UserName );
                    send_files();
                    share->showSharedByMe();
                    share->showSharedWithMe();
                    break;
            case 21: sync->AllFilesFromClient ( UserName );
                    sqlite3_open("MyDropbox.db", &db);
                    strcpy ( sql, "UPDATE DROPBOX SET DELETED = 1 where DELETED = 2 AND USERNAME = " );
                    strcat ( sql, "\'");
                    strcat ( sql, UserName );
                    strcat ( sql, "\'" );
                    sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
                    sqlite3_close ( db );
                    send_files();
                    break;
            case 22: DeleteFile();
                    send_files();
                    break;
            case 25: share->setPrivelege();
                    //share->showSharedByMe();
                    //share->showSharedWithMe();
                    break;
            case 26: share->changePrivelege();
                    share->showSharedByMe();
                    share->showSharedWithMe();
                    break;
            case 27: share->unShare();
                    share->showSharedByMe();
                    share->showSharedWithMe();
                    break;
            case 28: DownloadSharedFile();
                    break;
            case 40: SSL_free ( ssl ); 
                    close ( ClientSocket );
                    break;
        }
    }
}

void Server_dropbox::DownloadSharedFile () {
    int status;
    char file[255], sharer[255];
    int priv;
    std::cout<<"Privlege change reached"<<std::endl;

    while (1 ) {
        do {
            status = SSL_read ( ssl, &file, 256 );
        } while ( status < 0 );
        std::cout<<"Privlege : "<<file<<std::endl;
        if ( strcmp ( file , "" ) == 0 ) {
            break;
        }
        do {
            status = SSL_read ( ssl, &sharer, 256 );
        } while ( status < 0 );
        std::cout<<"Privlege : "<<sharer<<std::endl;
        do {
            status = SSL_read ( ssl, &priv, 4 );
        } while ( status < 0 );
        std::cout<<"Privlege : "<<priv<<std::endl;
        char *file_id, *last_edit, *ext;
        sqlite3 *db;
        char *zErrMsg = 0;
        char sql[255];
        char last_edit_2[255];
        sqlite3_open("MyDropbox.db", &db);
        strcpy ( sql, "SELECT FILEID from DROPBOX where USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND SHARED_FROM = ");
        strcat ( sql, "\'" );
        strcat ( sql, sharer );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        cout<<sql<<endl;
        sqlite3_exec(db, sql, callback4, (void*)&file_id, &zErrMsg);
        char file_id_name[255];
        strcpy ( file_id_name, file_id );
        strcpy ( sql, "SELECT EXTENSION from DROPBOX where USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND SHARED_FROM = ");
        strcat ( sql, "\'" );
        strcat ( sql, sharer );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        cout<<sql<<endl;
        sqlite3_exec(db, sql, callback4, (void*)&ext, &zErrMsg);
        strcat ( file_id_name, ext );
        cout<<file_id_name<<endl;
        strcpy ( sql, "SELECT LAST_EDITED from DROPBOX where USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, file );
        strcat ( sql, "\'" );
        strcat ( sql, " AND SHARED_FROM = ");
        strcat ( sql, "\'" );
        strcat ( sql, sharer );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, "/Shared/" );
        strcat ( sql, "\'" );
        cout<<sql<<endl;
        sqlite3_exec(db, sql, callback4, (void*)&last_edit, &zErrMsg);
        strcpy ( last_edit_2, last_edit );
        sqlite3_close (db);
        SendFile ( file, file_id_name, "/Shared/", last_edit_2 );
    }
}

void Server_dropbox::establish_server( int port ){
    CreateServerSocket( port );
    while (server_connect) {
	    Server_client_connection();
	}
}

void Server_dropbox::login_register_screen () {
    int status;
    int option;
    do {
        status = SSL_read ( ssl, &option, 4 );
    } while ( status < 0 );
    if ( option == 1 ) {

        ServerRegister serverregister ( ClientSocket, ssl );
        serverregister.registerAccount ();
    }
    else {
        ServerLogin serverlogin (ClientSocket, ssl );
        logged_in = serverlogin.LoginAccount( UserName );
    }
}

void Server_dropbox::SendFile ( char* Name, char* FileIdExt, char* Path, char* EditHis ) {
    int status;
    const char* fname;
    const char* fpath;
    const char* edit_last;
    char FilePath[256];
    fpath = Path;
    do{
        status = SSL_write( ssl, fpath, 256);
    } while (status < 0);

    fname = Name;               //Filename on client side with extention
    do{
        status = SSL_write( ssl, fname, 256);
    } while (status < 0);

    edit_last = EditHis;
    cout << edit_last << endl;
    do{
        status = SSL_write( ssl, edit_last, 256);
    } while (status < 0);


    //cout << fname << endl;
    strcpy ( FilePath, "Files/");
    strcat ( FilePath, FileIdExt );

    cout << FilePath << endl;
    //fname = FilePath;           //Filename with path on server side
    FILE *File;
    File = fopen ( FilePath, "r" );
    if ( File == NULL ) {
        cout << "File not on Server\n";
        return;
    }
   // const char* last_edit = Lastedit;
   // do{
  //      status = SSL_write( ssl, last_edit, 256);
  //  } while (status < 0);
    fseek( File, 0, SEEK_END );
    int FileSize = ftell( File );
    fseek( File, 0, SEEK_SET );
    do {
        status = SSL_write ( ssl, (void*)&FileSize, 4 );
    } while ( status < 0 );
    int read_size;
    char send_buffer[20481];
    int transferred = 0;
    //cout << "0 percent\n";
    while(!feof(File)) {
        read_size = fread ( send_buffer, 1, sizeof(send_buffer)-1, File );
        do{
            status = SSL_write( ssl, send_buffer, read_size);
        } while (status < 0);
        bzero(send_buffer, sizeof(send_buffer));
        transferred += read_size;
        //cout << (int)(((float)recv_size/FileSize)*100) << " percent\n";
    }
    fclose ( File );
    #ifdef DEBUG_SYNC
        cout << "File Successfully Sent!\n\n";
    #endif
}

void Server_dropbox::StoreFile () {
    int status;
    char ReadBuffer[256],last_edit[256],path_name[256], FileHash[255];
    int FileSize;
    bzero ( last_edit, sizeof ( last_edit ));
    do {
        status = SSL_read ( ssl, &last_edit, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received last edit "<<last_edit<<endl;
    #endif
    bzero ( ReadBuffer, sizeof ( ReadBuffer ));
    do {
        status = SSL_read ( ssl, &ReadBuffer, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received name "<<ReadBuffer<<endl;
    #endif
    char* fname = ReadBuffer ;          //fname contains filename.extension
          //filename has path to the user folder
    const char* lastedit = last_edit;
    bzero ( path_name, sizeof ( path_name ));
    do {
        status = SSL_read ( ssl, &path_name, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received path "<<path_name<<endl;
    #endif
    const char* pathname = path_name;
    do {
        status = SSL_read ( ssl, &FileHash, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received hash "<<FileHash<<endl;
    #endif
    do {
        status = SSL_read ( ssl, &FileSize, 4 );
    } while ( status < 0 );
    if ( FileSize == -1 )
        return;
    #ifdef DEBUG_SYNC
        cout<<"Filesize "<< FileSize << endl;
    #endif
    int fullstop;
    char *extension;
    for ( int i = 0; i < strlen(fname); i++ ) {
        if ( fname[i] == '.' ) fullstop = i;
    }
    extension = &fname[fullstop];
    char filename[256];
    strcpy ( filename, "Files/" );
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    char *file_id;
    char file_idenity[255];
    string sizeFile = to_string ( FileSize );
    const char* size_of_file = sizeFile.c_str();
    sqlite3_open("MyDropbox.db", &db);
    char *num_ind;
    strcpy ( sql, "SELECT COUNT(*) + 1 from DROPBOX where HASH = ");
    strcat ( sql, "\'" );
    strcat ( sql, FileHash );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILESIZE = ");
    strcat ( sql, "\'" );
    strcat ( sql, size_of_file );
    strcat ( sql, "\'" );
    strcat ( sql, " AND DELETED <> 0");
    #ifdef DEBUG_SQL
        cout<<sql<<endl;
    #endif
    char number_ind[255];
    bzero (number_ind, sizeof(number_ind));
    sqlite3_exec(db, sql, callback, (void*)&num_ind, &zErrMsg);
    #ifdef DEBUG_SYNC
        cout<<"Received num_id "<<num_ind<<endl;
    #endif
    strcpy ( number_ind, num_ind );
    if ( strcmp ( num_ind, "1" ) == 0 ) {
        strcpy ( sql, "SELECT MAX(FILEID)+1 FROM DROPBOX" );
        sqlite3_exec(db, sql, callback4, (void*)&file_id, &zErrMsg);
        #ifdef DEBUG_SYNC
            cout<<"Received file_id "<<file_id<<endl;
        #endif
        strcpy ( file_idenity, file_id );
        strcat ( filename, file_id );
        strcat ( filename, extension );
        //const char *fname = fname;
        FILE *File;
        File = fopen ( filename, "w" );
        if ( File == NULL ) {
            #ifdef DEBUG_SYNC
                cout << "File could not be created\n";
            #endif
            return;
        }
        #ifdef DEBUG_SYNC
            cout<<"ready to receive"<<endl;
        #endif
        timeval timeout = {10,0};
        fd_set fds;
        int buffer_fd;
        int recv_size = 0;
        int read_size, write_size;
        char FileArray[20480];
        int bc = 1;
        do {
            status = SSL_write ( ssl, &bc, 4 );
        } while ( status < 0 );
        while(recv_size < FileSize ) {
            FD_ZERO(&fds);
            FD_SET(ServerSocket,&fds);
            buffer_fd = select ( FD_SETSIZE, &fds, NULL, NULL, &timeout );
            if (buffer_fd < 0) {
                #ifdef DEBUG_SYNC
                    cout << "error: bad file descriptor set.\n";
                #endif
            }
            if (buffer_fd == 0) {
                #ifdef DEBUG_SYNC
                    cout << "error: buffer read timeout expired.\n";
                #endif
            }
            if (buffer_fd > 0)
            {
                do {
                    read_size = SSL_read ( ssl, FileArray, 20480 );
                } while ( read_size < 0 );
                write_size = fwrite ( FileArray, 1, read_size, File );
                recv_size += read_size;
            }
        }
        fclose ( File );
    }
    else {
        strcpy ( sql, "SELECT FILEID from DROPBOX where HASH = ");
        strcat ( sql, "\'" );
        strcat ( sql, FileHash );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILESIZE = ");
        strcat ( sql, "\'" );
        strcat ( sql, size_of_file );
        strcat ( sql, "\'" );
        sqlite3_exec(db, sql, callback4, (void*)&file_id, &zErrMsg);
        strcpy ( file_idenity, file_id );
        #ifdef DEBUG_SQL
            cout << "INCREASING OWNERS!!!!" << endl;
        #endif
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS + 1 where HASH = ");
        strcat ( sql, "\'" );
        strcat ( sql, FileHash );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILESIZE = ");
        strcat ( sql, "\'" );
        strcat ( sql, size_of_file );
        strcat ( sql, "\'" );
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        int bc=0;
        do {
            status = SSL_write ( ssl, &bc, 4 );
        } while ( status < 0 );
    }
    #ifdef DEBUG_SQL
        cout << file_id << endl;
    #endif
    strcpy ( sql, "INSERT INTO DROPBOX VALUES ( ");
    strcat ( sql, "\'" );
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, fname );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, pathname );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, file_idenity);
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  extension );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  "1" );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  lastedit );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  "NULL" );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  size_of_file );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql,  FileHash );
    strcat ( sql, "\'" );
    strcat ( sql, ", " );
    strcat ( sql, "\'" );
    strcat ( sql, number_ind );
    strcat ( sql, "\'" );
    strcat ( sql, ");" );
    #ifdef DEBUG_SQL
        cout << sql << endl;
    #endif
    sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    sqlite3_close(db);
    #ifdef DEBUG_SYNC
        cout << "File Successfully Received!\n\n";
    #endif
}

int Server_dropbox::getClientSocket() {
    return ClientSocket;
}

void Server_dropbox::deleteSocket(){
    close ( ServerSocket );
}

void Server_dropbox::UpdateStoreFile() {
    int status;
    char ReadBuffer[256],last_edit[256],path_name[256], hashval[256];
    int FileSize;
    bzero ( last_edit, sizeof ( last_edit ));
    do {
        status = SSL_read ( ssl, &last_edit, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received last edit "<<last_edit<<endl;
    #endif    
    bzero ( ReadBuffer, sizeof ( ReadBuffer ));
    do {
        status = SSL_read ( ssl, &ReadBuffer, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received name "<<ReadBuffer<<endl;
    #endif    
    char* fname = ReadBuffer ;          //fname contains filename.extension
          //filename has path to the user folder
    const char* lastedit = last_edit;
    bzero ( path_name, sizeof ( path_name ));
    do {
        status = SSL_read ( ssl, &path_name, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received path "<<path_name<<endl;
    #endif    
    const char* pathname = path_name;
    bzero ( hashval, sizeof ( hashval ));
    do {
        status = SSL_read ( ssl, &hashval, 256 );
    } while ( status < 0 );
    #ifdef DEBUG_SYNC
        cout<<"received hash "<<hashval<<endl;
    #endif    
    char* hash = hashval ;
    do {
        status = SSL_read ( ssl, &FileSize, 4 );
    } while ( status < 0 );
    if ( FileSize == -1 )
        return;
    #ifdef DEBUG_SYNC
        cout<<"Filesize "<< FileSize << endl;
    #endif    
    int fullstop;
    char *extension;
    for ( int i = 0; i < strlen(fname); i++ ) {
        if ( fname[i] == '.' ) fullstop = i;
    }

    extension = &fname[fullstop];
    char filename[256];
    strcpy ( filename, "Files/" );
    char sql[255];
    sqlite3 *db;
    char *zErrMsg = 0;
    char *file_id;
    sqlite3_open("MyDropbox.db", &db);
    char *num_ind;
    strcpy ( sql, "SELECT OWNERS from DROPBOX where USERNAME = ");
    strcat ( sql, "\'" );
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ");
    strcat ( sql, "\'" );
    strcat ( sql, fname );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'" );
    strcat ( sql, pathname );
    strcat ( sql, "\'" );
    #ifdef DEBUG_SQL
        cout<<sql<<endl;
    #endif   
    sqlite3_exec(db, sql, callback4, (void*)&num_ind, &zErrMsg);
    char number_ind[255];
    strcpy ( number_ind, num_ind);
    #ifdef DEBUG_SQL
        cout<<"number_ind "<<number_ind<<endl;
    #endif    
    if ( strcmp ( number_ind, "1" ) != 0 ) {
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS - 1 where HASH = ( SELECT HASH FROM DROPBOX where USERNAME = " );
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, "AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, "AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, pathname );
        strcat ( sql, "\'" );
        strcat ( sql, ") AND FILESIZE = ( SELECT FILESIZE FROM DROPBOX where USERNAME = " );
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, "AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, "AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, pathname );
        strcat ( sql, "\'" );
        strcat ( sql, ") AND DELETED IN ( 1, 11 )" );
        #ifdef DEBUG_SYNC
            cout<<sql<<endl;
        #endif
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        strcpy ( sql, "UPDATE DROPBOX SET FILEID = ( SELECT MAX(FILEID)+1 from DROPBOX ), OWNERS = 1, HASH = ");
        strcat ( sql, "\'" );
        strcat ( sql, hashval );
        strcat ( sql, "\'" );
        strcat ( sql, " WHERE USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, pathname );
        strcat ( sql, "\'" );
        strcat ( sql, ";" );
        #ifdef DEBUG_SYNC
            cout<<sql<<endl;
        #endif
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
    strcpy ( sql, "SELECT FILEID FROM DROPBOX WHERE USERNAME = " );
    strcat ( sql, "\'" );
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    strcat ( sql, "AND FILENAME = ");
    strcat ( sql, "\'" );
    strcat ( sql, fname );
    strcat ( sql, "\'" );
    strcat ( sql, "AND FILEPATH = ");
    strcat ( sql, "\'" );
    strcat ( sql, pathname );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    sqlite3_exec(db, sql, callback, &file_id, &zErrMsg);
    strcat ( filename, file_id );
    strcat ( filename, extension );
    //const char *fname = fname;
    FILE *File;
    File = fopen ( filename, "w" );
    if ( File == NULL ) {
        #ifdef DEBUG_SYNC
            cout << "File could not be created\n";
        #endif
        return;
    }
    timeval timeout = {10,0};
    fd_set fds;
    int buffer_fd;
    int recv_size = 0;
    int read_size, write_size;
    char FileArray[20480];
    int bc = 1;
    do {
        status = SSL_write ( ssl, &bc, 4 );
    } while ( status < 0 );
    while(recv_size < FileSize ) {
        FD_ZERO(&fds);
        FD_SET(ServerSocket,&fds);
        buffer_fd = select ( FD_SETSIZE, &fds, NULL, NULL, &timeout );
        if (buffer_fd < 0) {
            #ifdef DEBUG_SYNC
                cout << "error: bad file descriptor set.\n";
            #endif
        }
        if (buffer_fd == 0){
            #ifdef DEBUG_SYNC
                cout << "error: buffer read timeout expired.\n";
            #endif
        }
        if (buffer_fd > 0)
        {
            do {
                read_size = SSL_read ( ssl, FileArray, 20480 );
            } while ( read_size < 0 );
            write_size = fwrite ( FileArray, 1, read_size, File );
            recv_size += read_size;
        }
    }
    fclose ( File );
    #ifdef DEBUG_SYNC
        cout<<"File received "<<endl;
    #endif
    sqlite3_open("MyDropbox.db", &db);
    strcpy ( sql , "UPDATE DROPBOX SET DELETED = 2, LAST_EDITED = ");
    strcat ( sql, "\'" );
    strcat ( sql, lastedit);
    strcat ( sql, "\'");
    strcat ( sql , " WHERE USERNAME = " );
    strcat ( sql, "\'" );
    strcat ( sql, UserName );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILENAME = ");
    strcat ( sql, "\'" );
    strcat ( sql, fname );
    strcat ( sql, "\'" );
    strcat ( sql, " AND FILEPATH = ");
    strcat ( sql, "\'" );
    strcat ( sql, pathname );
    strcat ( sql, "\'" );
    strcat ( sql, ";" );
    #ifdef DEBUG_SYNC
        cout<<sql<<endl;
    #endif
    sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    sqlite3_close ( db  );
}

void Server_dropbox::DeleteFile(){

    while (1) {
        int status;
        char ReadBuffer[256],last_edit[256],path_name[256], FileHash[255];
        int FileSize;
        bzero ( last_edit, sizeof ( last_edit ));
        do {
            status = SSL_read ( ssl, &last_edit, 256 );
        } while ( status < 0 );
        if (strcmp ( last_edit, "") == 0 ) {
            break;
        }
        #ifdef DEBUG_SYNC
            cout<<"received last edit "<<last_edit<<endl;
        #endif
        bzero ( ReadBuffer, sizeof ( ReadBuffer ));
        do {
            status = SSL_read ( ssl, &ReadBuffer, 256 );
        } while ( status < 0 );
        #ifdef DEBUG_SYNC
            cout<<"received name "<<ReadBuffer<<endl;
        #endif
        char* fname = ReadBuffer ;          //fname contains filename.extension
              //filename has path to the user folder
        const char* lastedit = last_edit;
        bzero ( path_name, sizeof ( path_name ));
        do {
            status = SSL_read ( ssl, &path_name, 256 );
        } while ( status < 0 );
             cout<<"received path "<<path_name<<endl;
        char sql[255];
        sqlite3 *db;
        char *zErrMsg = 0;
        sqlite3_open("MyDropbox.db", &db);
        strcpy ( sql, "SELECT FILEID, EXTENSION, USERNAME FROM DROPBOX where DELETED = 1 AND OWNERS = 1 AND USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName);
        strcat ( sql, "\'");
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, path_name );
        strcat ( sql, "\'" );
        strcat ( sql, ";" );
        cout<<sql<<endl;
        sqlite3_exec(db, sql, callback5, 0, &zErrMsg);
        strcpy ( sql, "UPDATE DROPBOX SET DELETED = 0, LAST_EDITED = ");
        strcat ( sql, "\'" );
        strcat ( sql, last_edit);
        strcat ( sql, "\'" );
        strcat ( sql , " where DELETED = 1 AND USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName);
        strcat ( sql, "\'");
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, path_name );
        strcat ( sql, "\'" );
        strcat ( sql, ";" );
        cout<<sql<<endl;
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        strcpy ( sql, "UPDATE DROPBOX SET OWNERS = OWNERS - 1 where DELETED <> 0 AND FILEID = (SELECT FILEID FROM DROPBOX WHERE DELETED = 0 AND USERNAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, UserName);
        strcat ( sql, "\'");
        strcat ( sql, " AND FILENAME = ");
        strcat ( sql, "\'" );
        strcat ( sql, fname );
        strcat ( sql, "\'" );
        strcat ( sql, " AND FILEPATH = ");
        strcat ( sql, "\'" );
        strcat ( sql, path_name );
        strcat ( sql, "\'" );
        strcat ( sql, ");" );
        cout<<"Delete Query done "<<sql<<endl;
        sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }
}

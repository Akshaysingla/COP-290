#ifndef SERVER_DROPBOX_H
#define SERVER_DROPBOX_H

#include <openssl/ssl.h>
#include <openssl/err.h>


class Server_dropbox{
    public :
    Server_dropbox();
    void CreateServerSocket(int);
    void deleteSocket();
    void StoreFile ();
    void UpdateStoreFile();
    void SendFile( char* Name, char* FileId , char* Path , char*);
    int getClientSocket();
    void Server_client_connection();
    void establish_server(int);
    void client_request();
    void login_register_screen();
    void sync_initial_data();
    void send_files();
    void DeleteFile();
    void DownloadSharedFile();
private:
    int ServerSocket;
    int ClientSocket;
    int SocketSize;
    struct sockaddr_in Server, Client;
    bool server_connect;
    int logged_in;
    char UserName[256];
    SSL_CTX *ctx;
    SSL *ssl;
};

#endif // SERVER_DROPBOX_H
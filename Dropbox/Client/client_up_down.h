#ifndef CLIENT_UP_DOWN_H
#define CLIENT_UP_DOWN_H

#include "../GUI/Start_Window/progressbar.h"
#include <string.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <string>

class Client_up_down{
    public :
    Client_up_down(int sock, SSL *sslt );
    void UploadFile (char*,char*,std::string);
    void UploadFile_2 (std::string,const char*,std::string);
    void DownloadFile (const char*,char*);
    void SplitFilename (std::string str,std::string* filepath,std::string);
    char* check_ls_file(char*);
    void write_ls_file(char*, char*);
    std::string create_sha1(const char *name);

private:
    int client_socket;
    ProgressBar *progressbar;
    SSL *ssl;
};

#endif // CLIENT_UP_DOWN_H

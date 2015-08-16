
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
#include <string>
#include <stdlib.h>
#include "client_up_down.h"
#include <QApplication>

using namespace std;
Client_up_down::Client_up_down ( int sock, SSL *sslt ) {
    client_socket = sock;
    ssl = sslt;
}

void Client_up_down::SplitFilename (string str,string* filepath,string homepath)
{

    size_t found_1 = str.find(homepath);
    if (found_1 == 0){
        unsigned found = str.substr(found_1 + homepath.length()).find_last_of("/");
        filepath[0] = str.substr(found_1 + homepath.length()).substr(0,found+1);
        filepath[1] = str.substr(found_1 + homepath.length()).substr(found+1) ;
    }
    else{
        unsigned found = str.find_last_of("/");
        filepath[0] = "/";
        filepath[1] = str.substr(found+1) ;
    }
}

void Client_up_down::UploadFile_2(string filename,const char* datetime,string homepath) {
    int status;


    do {
        status = SSL_write( ssl, datetime, 256 );
    } while ( status < 0 );
    std::string *filepath = new std::string[2];
    SplitFilename (filename,filepath,homepath);
    const char* filename_1 = filepath[1].c_str();
    do {
        status = SSL_write( ssl, filename_1 , 256 );
    } while ( status < 0 );
    int instruct;
    const char* filepath_1 = filepath[0].c_str();
    do {
        status = SSL_write( ssl, filepath_1, 256 );
    } while ( status < 0 );
    const char* FileName = filename.c_str();

    std::string s_hash =  create_sha1(FileName);
    const char* hash = s_hash.c_str();
    do {
        status = SSL_write( ssl, hash, 256 );
    } while ( status < 0 );
    std::cout << hash << std::endl;
    FILE *File;
    File = fopen ( FileName, "r" );
        cout <<"Filename "<< filename << endl;
    if ( File == NULL ) {
        std::cout << "No such file!\n";

        int nofile = -1;
        do {
            status = SSL_write( ssl, (void*)&nofile, 4 );
        } while ( status < 0 );
        return;
    }
    fseek( File, 0, SEEK_END );
    int FileSize = ftell( File );
    fseek( File, 0, SEEK_SET );
    do {
        status = SSL_write( ssl, (void*)&FileSize, 4 );
    } while ( status < 0 );

    int bc;
    do {
        status = SSL_read( ssl, (void*)&bc, 4 );
    } while ( status < 0 );

    if (bc == 1){


    int read_size;
    char send_buffer[20480];
    int transferred = 0;
    int percent = 0;
    progressbar = new ProgressBar;
    progressbar->show();
    while(!feof(File)) {
        read_size = fread ( send_buffer, 1, sizeof(send_buffer)-1, File );
        do{
            status = SSL_write( ssl, send_buffer, read_size);
        } while (status < 0);
        bzero(send_buffer, sizeof(send_buffer));
        transferred += read_size;
        percent = (int)(((float)transferred/FileSize)*100);
        progressbar->setvalue(percent);
        QApplication::processEvents();
        if(percent==100){
            progressbar->close();
        }
    }
    }
    fclose ( File );
    std::cout << "File Transferred Successfully\n\n";
}

void Client_up_down::UploadFile(char* temp,char* datetime,string homepath) {
    const char* FileName = temp;
    int status;
    int option = 1;
    string* filepath = new string[2];
    SplitFilename (temp,filepath,homepath);
    do {
        status = SSL_write( ssl, (void*)&option, 4 );
    } while ( status < 0 );
    
    //send filename
    const char* nName = filepath[1].c_str();
    do {
        status = SSL_write( ssl, nName , 256 );
    } while ( status < 0 );
    //send file modified
    do {
        status = SSL_write( ssl, datetime, 256 );
    } while ( status < 0 );
    
    //send file path
    nName = filepath[0].c_str();
    cout << nName << endl;
    do {
        status = SSL_write( ssl, nName, 256 );
    } while ( status < 0 );

    std::string s_hash;
    s_hash.assign(create_sha1(FileName));
    const char* hash = s_hash.c_str();
    do {
        status = SSL_write( ssl, hash, 256 );
    } while ( status < 0 );
    
    FILE *File;
    File = fopen ( FileName, "r" );
    if ( File == NULL ) {
        std::cout << "No such file!\n";

        int nofile = -1;
        do {
            status = SSL_write( ssl, (void*)&nofile, 4 );
        } while ( status < 0 );

        return;
    }
    fseek( File, 0, SEEK_END );
    int FileSize = ftell( File );
    fseek( File, 0, SEEK_SET );
    do {
        status = SSL_write( ssl, (void*)&FileSize, 4 );
    } while ( status < 0 );
    int read_size;
    char send_buffer[20480];
    int transferred = 0;
    int percent = 0;
    progressbar = new ProgressBar;
    progressbar->show();
    while(!feof(File)) {
        read_size = fread ( send_buffer, 1, sizeof(send_buffer)-1, File );
        do{
            status = SSL_write( ssl, send_buffer, read_size);
        } while (status < 0);
        bzero(send_buffer, sizeof(send_buffer));
        transferred += read_size;
        percent = (int)(((float)transferred/FileSize)*100);
        progressbar->setvalue(percent);
        QApplication::processEvents();
    }
    fclose ( File );
    std::cout << "File Transferred Successfully\n\n";
}

void Client_up_down::DownloadFile ( const char* temp,char* lastedit) {
    const char* FileName = temp;
    int status;
    int FileSize;
    do {
        status = SSL_read( ssl, &FileSize, 4 );
    } while ( status < 0 );
    FILE *File;
    File = fopen ( FileName, "w" );
    if ( File == NULL ) {
        std::cout << "File cannot be created\n";
        return;
    }
    timeval timeout = {10,0};
    fd_set fds;
    int buffer_fd;
    int recv_size = 0;
    int read_size, write_size;
    char FileArray[20481];
    //cout << "0 percent\n";
    int percent = 0;
    ProgressBar prog;
    prog.show();
    while(recv_size < FileSize ) {
        FD_ZERO(&fds);
        FD_SET(client_socket,&fds);
        buffer_fd = select ( FD_SETSIZE, &fds, NULL, NULL, &timeout );
        if (buffer_fd < 0)
            std::cout << "error: bad file descriptor set.\n";
        if (buffer_fd == 0)
            std::cout << "error: buffer read timeout expired.\n";
        if (buffer_fd > 0)
        {
            do {
                read_size = SSL_read( ssl, FileArray, 20481 );
            } while ( read_size < 0 );

            write_size = fwrite ( FileArray, 1, read_size, File );
            recv_size += read_size;
            percent = (int)(((float)recv_size/FileSize)*100);
            prog.setvalue(percent);
            QApplication::processEvents();
        }
    }

    fclose ( File );
    char timemode[512] = "touch -t ";
    strcat(timemode,lastedit);
    strcat(timemode," ");
    strcat(timemode,temp);
    const char* stimemode = timemode;
    system(stimemode);
    std::cout << "File Successfully Downloaded!\n\n";
}



char* Client_up_down::check_ls_file(char *pathname){
    char s_time[255] ;
    strcpy ( s_time,  "0" );
    ifstream myfile (pathname);
    if (!myfile.good()) { 
        return s_time;
    } 
    else {
        myfile >> s_time; myfile.close(); 
        return s_time;
    }
} 

void Client_up_down::write_ls_file(char* pathname , char* lsync){
    ofstream myfile (pathname); 
        myfile << lsync;
        myfile.close(); 
} 

string Client_up_down::create_sha1(const char *name)
{
	unsigned char hash[SHA_DIGEST_LENGTH];
    FILE *f;
    unsigned char buf[8192];
    SHA_CTX ctx;

    f = fopen(name, "rb");
    SHA1_Init(&ctx);
    for (;;) {
        size_t len;

        len = fread(buf, 1, sizeof buf, f);
        if (len == 0)
            break;
        SHA1_Update(&ctx, buf, len);
    }

    char const hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    
    fclose(f);
    SHA1_Final(hash, &ctx);
    string temp;
    for (int i=0 ;i<20;i++){
    	const char ch = hash[i];
        temp.append(&hex[(ch & 0xF0)>>4],1);
        temp.append(&hex[(ch & 0xF)],1);
    }
    return temp;
}


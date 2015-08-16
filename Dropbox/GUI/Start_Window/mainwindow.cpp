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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../Client/SSL_Client.h"
#include <QDesktopWidget>

MainWindow::MainWindow(char* ip_addr, int portno, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());
    char buf[1024];
    int bytes;
    SSL_library_init();
    ctx = InitCTX();
    clientdropbox = new Client_dropbox(ip_addr, portno );
    client_socket = clientdropbox->CreateSocketToServer();
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client_socket);
    if ( SSL_connect(ssl) == -1 )
           ERR_print_errors_fp(stderr);
       else
       {
           printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
           ShowCerts(ssl);
       }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    sendSignal(0);
    loginscreen = new login_screen(client_socket, ssl, this, ctx);
    loginscreen->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    sendSignal(1);
    registerscreen = new register_screen(client_socket, ssl);
    registerscreen->show();
}

void MainWindow::sendSignal ( int option ) {
    int status;
    do {
        status = SSL_write ( ssl, &option, 4 );
    } while ( status < 0 );
}

void MainWindow::on_commandLinkButton_clicked()
{
    readme = new ReadMe("../../Readme.txt");
    readme->show();
}

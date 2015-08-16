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
#include "register_screen.h"
#include "ui_register_screen.h"
#include "error_window.h"
#include "../../Client/Client_register.h"
#include <QDesktopWidget>

register_screen::register_screen(int sock, SSL *sslt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::register_screen)
{
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());
    client_socket = sock;
    ssl = sslt;
}

register_screen::~register_screen()
{
    this->close();
    int status;
    char *cancel = "";
    do {
        status = SSL_write ( ssl, cancel, 1 );
    } while ( status < 0 );
    delete ui;
}

void register_screen::on_pushButton_clicked() {
    QString user, pass, fname, sname, cpass;
    Error_window *errorwindow;
    fname = ui->lineEdit->text();
    sname = ui->lineEdit_2->text();
    user = ui->lineEdit_3->text();
    pass = ui->lineEdit_4->text();
    cpass = ui->lineEdit_5->text();
    QByteArray id = user.toLocal8Bit();
    QByteArray pword = pass.toLocal8Bit();
    QByteArray name = fname.toLocal8Bit();
    QByteArray lname = sname.toLocal8Bit();
    QByteArray cpword = cpass.toLocal8Bit();
    char *username = id.data();
    char *password = pword.data();
    char *firstname = name.data();
    char *lastname = lname.data();
    char *matchpassword = cpword.data();
    if ( strcmp ( username, "" ) == 0 ) {
        errorwindow = new Error_window("Username Required");
        errorwindow->show();
    }
    else if ( strcmp ( password, "" ) == 0 ) {
        errorwindow = new Error_window("Password Required");
        errorwindow->show();
    }
    else if ( strcmp ( firstname, "" ) == 0 ) {
        errorwindow = new Error_window("First Name Required");
        errorwindow->show();
    }
    else if ( strcmp ( lastname, "" ) == 0 ) {
        errorwindow = new Error_window("Last Name Required");
        errorwindow->show();
    }
    else {
        ClientRegister clientregister( username, password, firstname, lastname, client_socket, ssl );
        int errorcheck = clientregister.checkPassword( matchpassword );
        if(errorcheck == -2) {
            errorwindow = new Error_window("Atleast 6 characters required in password");
            errorwindow->show();
        }
        else if(errorcheck == -1) {
            errorwindow = new Error_window("Passwords entered do not match");
            errorwindow->show();
        }
        else {
            if( clientregister.registerAccount() == 0 ) {
                errorwindow = new Error_window("Username already exists");
                errorwindow->show();
            }
            else
                this -> close();
        }
    }
}
void register_screen::on_pushButton_2_clicked() {
    this->close();
    int status;
    char *cancel = "";
    do {
        status = SSL_write ( ssl, cancel, 1 );
    } while ( status < 0 );
}

#include "login_screen.h"
#include "ui_login_screen.h"
#include "error_window.h"
#include <unistd.h>
#include <string.h>
#include "../../Client/Client_login.h"
#include <iostream>
#include <QDesktopWidget>


login_screen::login_screen(int sock, SSL *sslt, QMainWindow* mainwin, SSL_CTX *ctxt, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login_screen)
{
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());
    client_socket = sock;
    mainwindow = mainwin;
    ctx = ctxt;
    ssl = sslt;
}

login_screen::~login_screen()
{
    this->close();
    int status;
    char *cancel = "";
    do {
        status = SSL_write ( ssl, cancel, 1 );
    } while ( status < 0 );
    delete ui;
}

void login_screen::on_pushButton_clicked()
{
    QString user, pass;
    Error_window *errorwindow;
    user = ui->lineEdit_2->text();
    pass = ui->lineEdit->text();
    QByteArray id = user.toLocal8Bit();
    QByteArray pword = pass.toLocal8Bit();
    char *username = id.data();
    char *password = pword.data();
    if ( strcmp ( username, "" ) == 0 ) {
        errorwindow = new Error_window("Username Required");
        errorwindow->show();
    }
    else if ( strcmp ( password, "" ) == 0 ) {
        errorwindow = new Error_window("Password Required");
        errorwindow->show();
    }
    else {
        ClientLogin clientlogin(username, password, client_socket, ssl);
        bool LoginStatus;
        LoginStatus = clientlogin.Login();
        if (LoginStatus){
            mainwindow->close();
            this->close();
            userui = new UserUI_1(client_socket, ssl, user, ctx );
            userui->show();
        }
        else {
            errorwindow = new Error_window("Username or Password incorrect");
            errorwindow->show();
        }
    }
}

void login_screen::on_pushButton_2_clicked() {
    this->close();
    int status;
    char *cancel = "";
    do {
        status = SSL_write ( ssl, cancel, 1 );
    } while ( status < 0 );
}

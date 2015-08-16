#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <login_screen.h>
#include <register_screen.h>
#include "../../Client/Client_dropbox.h"
#include <openssl/ssl.h>
#include "readme.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(char*, int, QWidget *parent = 0);
    ~MainWindow();
    void sendSignal ( int option );
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::MainWindow *ui;
    login_screen *loginscreen;
    register_screen *registerscreen;
    Client_dropbox *clientdropbox;
    int client_socket;
    SSL_CTX *ctx;
    SSL *ssl;
    ReadMe *readme;
};

#endif // MAINWINDOW_H

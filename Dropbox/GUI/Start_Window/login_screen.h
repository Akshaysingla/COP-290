#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include <QWidget>
#include <userui_1.h>
#include <qmainwindow.h>
#include <openssl/ssl.h>

namespace Ui {
class login_screen;
}

class login_screen : public QWidget
{
    Q_OBJECT
    
public:
    explicit login_screen(int sock, SSL *sslt, QMainWindow* mainwin, SSL_CTX *ctxt, QWidget *parent = 0);
    ~login_screen();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::login_screen *ui;
    UserUI_1 *userui;
    int client_socket;
    SSL *ssl;
    SSL_CTX *ctx;
    QMainWindow* mainwindow;
};

#endif // LOGIN_SCREEN_H

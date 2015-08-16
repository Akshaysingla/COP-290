#ifndef REGISTER_SCREEN_H
#define REGISTER_SCREEN_H

#include <QWidget>
#include <openssl/ssl.h>

namespace Ui {
class register_screen;
}

class register_screen : public QWidget
{
    Q_OBJECT
    
public:
    explicit register_screen(int sock, SSL *sslt, QWidget *parent = 0);
    ~register_screen();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::register_screen *ui;
    int client_socket;
    SSL *ssl;
};

#endif // REGISTER_SCREEN_H

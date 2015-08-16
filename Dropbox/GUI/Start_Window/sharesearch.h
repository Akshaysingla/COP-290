#ifndef SHARESEARCH_H
#define SHARESEARCH_H

#include <QDialog>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace Ui {
class ShareSearch;
}

class ShareSearch : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShareSearch(int sock, SSL *sslt, SSL_CTX *ctxt,QStringList ,QString,QString, QWidget *parent = 0);
    ~ShareSearch();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void Share(QString , QString , QString , int );

private:
    Ui::ShareSearch *ui;
    QStringListModel *m_model;
    QSortFilterProxyModel *m_filter;
    QStringList data;
    QString filename;
    QString filepath;
    SSL *ssl;
    int *sig;
    SSL_CTX *ctx;
    int client_socket;
};

#endif // SHARESEARCH_H

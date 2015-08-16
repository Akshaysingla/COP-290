#ifndef USERUI_1_H
#define USERUI_1_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include "../../Client/client_up_down.h"
#include <progressbar.h>
#include <openssl/ssl.h>
#include <QFileSystemModel>
#include <string>
#include <QDirModel>
#include <QTableWidget>
#include "sharesearch.h"
#include "readme.h"

namespace Ui {
class UserUI_1;
}

class UserUI_1 : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit UserUI_1(int sock, SSL *sslt, QString, SSL_CTX *ctxt, QWidget *parent = 0);
    void SyncAll(QString);
    ~UserUI_1();
    
private slots:
    void on_pushButton_clicked();

    //v//oid on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void refresh_data();

    void ServerList();

    void SharedByList();

    void SharedWithList();

    void SyncDirectory(QString);

    void SyncServerClient(QString);

    void SendFileInfo(QString);

    void ChangePriveledgeShare(QString  , QString , int );

    void Unshare(QString  , QString , int );

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_commandLinkButton_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::UserUI_1 *ui;
    int client_socket;
    int ServerRow;
    int SharedRow1;
    int SharedRow2;
    QStringListModel *model;
    QStringList browser_files;
    QStringListModel *model_2;
    QDirModel *model_1;
    QString user_name;
    Client_up_down *client_up_down;
    ProgressBar *prog;
    SSL *ssl;
    SSL_CTX *ctx;
    QString homeqpath;
    char homepath[255];
    QTableView table1;
    QTableView table2;
    QTableView table3;
    ShareSearch *sharewin;
    QStringList data;
    ReadMe *readme1;
};

#endif // USERUI_1_H

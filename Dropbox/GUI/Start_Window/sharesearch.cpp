#include "sharesearch.h"
#include "ui_sharesearch.h"
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QDesktopServices>
#include <QMessageBox>


ShareSearch::ShareSearch(int sock, SSL *sslt, SSL_CTX *ctxt,QStringList data1,QString f,QString p,QWidget *parent) :

    QDialog(parent),

    ui(new Ui::ShareSearch)
{
    ui->setupUi(this);
    ssl = sslt;
    ctx = ctxt;
    client_socket = sock;
    filename = f;
    filepath = p;
    data = data1;
    m_model = new QStringListModel(this);
    m_model->setStringList(data);
    m_filter = new QSortFilterProxyModel(this);
    m_filter->setSourceModel(m_model);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), m_filter, SLOT(setFilterFixedString(QString)));
    m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->listView->setModel(m_filter);
    
}

ShareSearch::~ShareSearch()
{
    *sig = 1;
    delete ui;
}

void ShareSearch::on_pushButton_clicked()
{
    QStringList sharee;
    sharee = m_model->stringList();
    if (sharee.size() == 0){
        QMessageBox::information(this,"","Select Username from the list");
    }
    else{
    int status;
    int instruct = 25;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    Share(filename,filepath,sharee.at(0),0);
    this->close();
    }

}

void ShareSearch::on_pushButton_2_clicked()
{
    QStringList sharee;
    sharee = m_model->stringList();
    if (sharee.size() == 0){
        QMessageBox::information(this,"","Select Username from the list");
    }
    else{
    int status;
    int instruct = 25;
    do {
        status = SSL_write( ssl, &instruct, 4 );
    } while ( status < 0 );
    Share(filename,filepath,sharee.at(0),1);
    this->close();
    }
}

void ShareSearch::on_pushButton_3_clicked()
{
    this->close();
}

void ShareSearch::Share(QString file, QString path, QString Sharee, int priv){
    int status;
    const char* file_name = file.toStdString().c_str();
    do {
        status = SSL_write( ssl, file_name, 256 );
    } while ( status < 0 );

    const char* file_path = path.toStdString().c_str();
    do {
        status = SSL_write( ssl, file_path, 256 );
    } while ( status < 0 );
    const char* sharee = Sharee.toStdString().c_str();
    do {
        status = SSL_write( ssl, sharee, 256 );
    } while ( status < 0 );
    do {
        status = SSL_write( ssl, &priv, 4 );
    } while ( status < 0 );
}

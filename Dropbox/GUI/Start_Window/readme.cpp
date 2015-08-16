#include "readme.h"
#include "ui_readme.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


ReadMe::ReadMe(QString URL,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadMe)
{
    ui->setupUi(this);
    QFile file(URL);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(0,"info",file.errorString());
        this->close();
    }
    QTextStream in(&file);
    ui->textBrowser->setText(in.readAll());
}

ReadMe::~ReadMe()
{
    delete ui;
}

void ReadMe::on_pushButton_clicked()
{
    this->close();
}

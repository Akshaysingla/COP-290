#include "foldername.h"
#include "ui_foldername.h"

FolderName::FolderName(QString* temp,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FolderName)
{
    ui->setupUi(this);
}

FolderName::~FolderName()
{
    delete ui;
}

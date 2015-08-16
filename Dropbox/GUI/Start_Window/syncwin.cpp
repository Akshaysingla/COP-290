#include "syncwin.h"
#include "ui_syncwin.h"

SyncWin::SyncWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncWin)
{
    ui->setupUi(this);
}

SyncWin::~SyncWin()
{
    delete ui;
}

#include "progressbar.h"
#include "ui_progressbar.h"
#include <QDesktopWidget>

ProgressBar::ProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressBar)
{
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());
    ui->progressBar->setValue(0);
}

void ProgressBar::setvalue( int val ) {
    ui->progressBar->setValue(val);
}

ProgressBar::~ProgressBar()
{
    this->close();
    delete ui;
}

void ProgressBar::close_bar() {
    this->close();
}

void ProgressBar::on_pushButton_clicked()
{
    this->close();
    delete ui;
}

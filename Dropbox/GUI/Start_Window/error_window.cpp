#include "error_window.h"
#include "ui_error_window.h"
#include <QDesktopWidget>

Error_window::Error_window(char* settext, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Error_window)
{
    ui->setupUi(this);
    this->move(QApplication::desktop()->screen()->rect().center()- this->rect().center());
    QString ErrorLabel = QString::fromLocal8Bit(settext);
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->label->setText(ErrorLabel);
}

void Error_window::on_pushButton_clicked() {
    this->close();
}

Error_window::~Error_window()
{
    delete ui;
}

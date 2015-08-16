/********************************************************************************
** Form generated from reading UI file 'progressbar.ui'
**
** Created: Mon Mar 2 17:59:21 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSBAR_H
#define UI_PROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProgressBar
{
public:
    QProgressBar *progressBar;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *ProgressBar)
    {
        if (ProgressBar->objectName().isEmpty())
            ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        ProgressBar->resize(503, 111);
        progressBar = new QProgressBar(ProgressBar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(47, 40, 411, 21));
        progressBar->setValue(24);
        label = new QLabel(ProgressBar);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 10, 91, 21));
        pushButton = new QPushButton(ProgressBar);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 70, 99, 27));

        retranslateUi(ProgressBar);

        QMetaObject::connectSlotsByName(ProgressBar);
    } // setupUi

    void retranslateUi(QWidget *ProgressBar)
    {
        ProgressBar->setWindowTitle(QApplication::translate("ProgressBar", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProgressBar", "Progress...", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ProgressBar", "Done", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgressBar: public Ui_ProgressBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSBAR_H

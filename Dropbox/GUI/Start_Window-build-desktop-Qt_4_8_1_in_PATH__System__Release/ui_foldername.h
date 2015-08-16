/********************************************************************************
** Form generated from reading UI file 'foldername.ui'
**
** Created: Tue Mar 3 23:02:48 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOLDERNAME_H
#define UI_FOLDERNAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FolderName
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *FolderName)
    {
        if (FolderName->objectName().isEmpty())
            FolderName->setObjectName(QString::fromUtf8("FolderName"));
        FolderName->resize(360, 271);
        label = new QLabel(FolderName);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(100, 30, 171, 17));
        lineEdit = new QLineEdit(FolderName);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(82, 60, 191, 27));
        pushButton = new QPushButton(FolderName);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(120, 146, 99, 31));
        pushButton_2 = new QPushButton(FolderName);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(120, 200, 99, 31));

        retranslateUi(FolderName);

        QMetaObject::connectSlotsByName(FolderName);
    } // setupUi

    void retranslateUi(QDialog *FolderName)
    {
        FolderName->setWindowTitle(QApplication::translate("FolderName", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FolderName", "Enter Folder Name", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FolderName", "Okay", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("FolderName", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FolderName: public Ui_FolderName {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOLDERNAME_H

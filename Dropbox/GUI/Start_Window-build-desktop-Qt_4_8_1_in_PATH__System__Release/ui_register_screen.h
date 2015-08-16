/********************************************************************************
** Form generated from reading UI file 'register_screen.ui'
**
** Created: Mon Mar 2 17:59:21 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_SCREEN_H
#define UI_REGISTER_SCREEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_register_screen
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;

    void setupUi(QWidget *register_screen)
    {
        if (register_screen->objectName().isEmpty())
            register_screen->setObjectName(QString::fromUtf8("register_screen"));
        register_screen->resize(513, 479);
        pushButton = new QPushButton(register_screen);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(260, 400, 99, 27));
        pushButton_2 = new QPushButton(register_screen);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(380, 400, 99, 27));
        layoutWidget = new QWidget(register_screen);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(90, 70, 130, 261));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_2->addWidget(label_4);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_2->addWidget(label_5);

        layoutWidget1 = new QWidget(register_screen);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(230, 60, 148, 281));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(layoutWidget1);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(layoutWidget1);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        verticalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(layoutWidget1);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        verticalLayout->addWidget(lineEdit_3);

        lineEdit_4 = new QLineEdit(layoutWidget1);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(lineEdit_4);

        lineEdit_5 = new QLineEdit(layoutWidget1);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(lineEdit_5);

        layoutWidget->raise();
        layoutWidget->raise();
        pushButton->raise();
        pushButton_2->raise();

        retranslateUi(register_screen);

        QMetaObject::connectSlotsByName(register_screen);
    } // setupUi

    void retranslateUi(QWidget *register_screen)
    {
        register_screen->setWindowTitle(QApplication::translate("register_screen", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("register_screen", "Register", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("register_screen", "Cancel", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("register_screen", "              First Name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("register_screen", "               Last Name:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("register_screen", "                Username:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("register_screen", "                 Password:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("register_screen", "Confirm Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class register_screen: public Ui_register_screen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_SCREEN_H

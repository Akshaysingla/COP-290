/********************************************************************************
** Form generated from reading UI file 'error_window.ui'
**
** Created: Tue Mar 3 23:42:57 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERROR_WINDOW_H
#define UI_ERROR_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Error_window
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *Error_window)
    {
        if (Error_window->objectName().isEmpty())
            Error_window->setObjectName(QString::fromUtf8("Error_window"));
        Error_window->resize(357, 109);
        label = new QLabel(Error_window);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 341, 51));
        pushButton = new QPushButton(Error_window);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(130, 70, 99, 27));

        retranslateUi(Error_window);

        QMetaObject::connectSlotsByName(Error_window);
    } // setupUi

    void retranslateUi(QWidget *Error_window)
    {
        Error_window->setWindowTitle(QApplication::translate("Error_window", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Error_window", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Error_window", "Okay", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Error_window: public Ui_Error_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERROR_WINDOW_H

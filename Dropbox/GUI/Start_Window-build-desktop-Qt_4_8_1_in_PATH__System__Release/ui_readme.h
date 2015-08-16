/********************************************************************************
** Form generated from reading UI file 'readme.ui'
**
** Created: Tue Mar 3 21:17:11 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_README_H
#define UI_README_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_ReadMe
{
public:
    QPushButton *pushButton;
    QTextBrowser *textBrowser;

    void setupUi(QDialog *ReadMe)
    {
        if (ReadMe->objectName().isEmpty())
            ReadMe->setObjectName(QString::fromUtf8("ReadMe"));
        ReadMe->resize(832, 716);
        pushButton = new QPushButton(ReadMe);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(360, 640, 99, 51));
        textBrowser = new QTextBrowser(ReadMe);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(25, 20, 771, 601));
        textBrowser->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse|Qt::TextBrowserInteraction|Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        retranslateUi(ReadMe);

        QMetaObject::connectSlotsByName(ReadMe);
    } // setupUi

    void retranslateUi(QDialog *ReadMe)
    {
        ReadMe->setWindowTitle(QApplication::translate("ReadMe", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ReadMe", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReadMe: public Ui_ReadMe {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_README_H

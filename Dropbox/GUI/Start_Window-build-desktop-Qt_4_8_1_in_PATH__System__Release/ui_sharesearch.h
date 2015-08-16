/********************************************************************************
** Form generated from reading UI file 'sharesearch.ui'
**
** Created: Tue Mar 3 20:18:13 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHARESEARCH_H
#define UI_SHARESEARCH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ShareSearch
{
public:
    QListView *listView;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QDialog *ShareSearch)
    {
        if (ShareSearch->objectName().isEmpty())
            ShareSearch->setObjectName(QString::fromUtf8("ShareSearch"));
        ShareSearch->resize(360, 493);
        listView = new QListView(ShareSearch);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(50, 70, 256, 311));
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);
        label = new QLabel(ShareSearch);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 30, 67, 17));
        lineEdit = new QLineEdit(ShareSearch);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(140, 26, 161, 31));
        pushButton = new QPushButton(ShareSearch);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(190, 400, 99, 27));
        pushButton_2 = new QPushButton(ShareSearch);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(60, 400, 99, 27));
        pushButton_3 = new QPushButton(ShareSearch);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(120, 440, 99, 27));

        retranslateUi(ShareSearch);

        QMetaObject::connectSlotsByName(ShareSearch);
    } // setupUi

    void retranslateUi(QDialog *ShareSearch)
    {
        ShareSearch->setWindowTitle(QApplication::translate("ShareSearch", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ShareSearch", "Search :", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ShareSearch", "Collaborate", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("ShareSearch", "Transfer", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("ShareSearch", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShareSearch: public Ui_ShareSearch {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHARESEARCH_H

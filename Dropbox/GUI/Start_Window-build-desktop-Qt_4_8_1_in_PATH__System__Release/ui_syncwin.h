/********************************************************************************
** Form generated from reading UI file 'syncwin.ui'
**
** Created: Tue Mar 3 23:39:49 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYNCWIN_H
#define UI_SYNCWIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_SyncWin
{
public:
    QLabel *label;

    void setupUi(QDialog *SyncWin)
    {
        if (SyncWin->objectName().isEmpty())
            SyncWin->setObjectName(QString::fromUtf8("SyncWin"));
        SyncWin->resize(552, 156);
        label = new QLabel(SyncWin);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(180, 30, 451, 71));

        retranslateUi(SyncWin);

        QMetaObject::connectSlotsByName(SyncWin);
    } // setupUi

    void retranslateUi(QDialog *SyncWin)
    {
        SyncWin->setWindowTitle(QApplication::translate("SyncWin", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SyncWin", "Syncing, please wait", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SyncWin: public Ui_SyncWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYNCWIN_H

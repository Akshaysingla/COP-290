/********************************************************************************
** Form generated from reading UI file 'userui_1.ui'
**
** Created: Tue Mar 3 23:39:49 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERUI_1_H
#define UI_USERUI_1_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserUI_1
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QCommandLinkButton *commandLinkButton;
    QPushButton *pushButton_20;
    QTabWidget *tabWidget_2;
    QWidget *tab_5;
    QPushButton *pushButton_13;
    QPushButton *pushButton_19;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QLabel *label_3;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QTableWidget *SharedList1;
    QTableWidget *SharedList2;
    QTableWidget *ServerList;
    QWidget *tab_6;
    QTreeView *treeView;
    QPushButton *pushButton_10;
    QListView *listView_2;
    QPushButton *pushButton;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QPushButton *pushButton_21;
    QPushButton *pushButton_8;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QMenuBar *menubar;
    QMenu *menuMyDropBOx;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *UserUI_1)
    {
        if (UserUI_1->objectName().isEmpty())
            UserUI_1->setObjectName(QString::fromUtf8("UserUI_1"));
        UserUI_1->resize(1405, 898);
        centralwidget = new QWidget(UserUI_1);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 30, 511, 61));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(970, 30, 111, 91));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../Icons/sync-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_5->setIcon(icon);
        pushButton_5->setIconSize(QSize(100, 100));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(1230, 30, 101, 91));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../../Icons/Logout-01.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_6->setIcon(icon1);
        pushButton_6->setIconSize(QSize(100, 100));
        commandLinkButton = new QCommandLinkButton(centralwidget);
        commandLinkButton->setObjectName(QString::fromUtf8("commandLinkButton"));
        commandLinkButton->setGeometry(QRect(840, 60, 91, 41));
        pushButton_20 = new QPushButton(centralwidget);
        pushButton_20->setObjectName(QString::fromUtf8("pushButton_20"));
        pushButton_20->setGeometry(QRect(1100, 30, 111, 91));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../../Icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_20->setIcon(icon2);
        pushButton_20->setIconSize(QSize(100, 100));
        tabWidget_2 = new QTabWidget(centralwidget);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(30, 170, 1301, 711));
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        pushButton_13 = new QPushButton(tab_5);
        pushButton_13->setObjectName(QString::fromUtf8("pushButton_13"));
        pushButton_13->setGeometry(QRect(1030, 530, 151, 41));
        pushButton_19 = new QPushButton(tab_5);
        pushButton_19->setObjectName(QString::fromUtf8("pushButton_19"));
        pushButton_19->setGeometry(QRect(1030, 590, 151, 41));
        pushButton_16 = new QPushButton(tab_5);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));
        pushButton_16->setGeometry(QRect(670, 500, 121, 41));
        pushButton_17 = new QPushButton(tab_5);
        pushButton_17->setObjectName(QString::fromUtf8("pushButton_17"));
        pushButton_17->setGeometry(QRect(670, 550, 121, 41));
        pushButton_18 = new QPushButton(tab_5);
        pushButton_18->setObjectName(QString::fromUtf8("pushButton_18"));
        pushButton_18->setGeometry(QRect(670, 600, 121, 41));
        label_3 = new QLabel(tab_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(660, 470, 191, 41));
        pushButton_7 = new QPushButton(tab_5);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(170, 570, 191, 71));
        pushButton_9 = new QPushButton(tab_5);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(170, 500, 191, 61));
        SharedList1 = new QTableWidget(tab_5);
        if (SharedList1->columnCount() < 3)
            SharedList1->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        SharedList1->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        SharedList1->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        SharedList1->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        SharedList1->setObjectName(QString::fromUtf8("SharedList1"));
        SharedList1->setGeometry(QRect(580, 20, 331, 451));
        SharedList1->setSelectionMode(QAbstractItemView::SingleSelection);
        SharedList1->setSelectionBehavior(QAbstractItemView::SelectRows);
        SharedList1->horizontalHeader()->setDefaultSectionSize(109);
        SharedList2 = new QTableWidget(tab_5);
        if (SharedList2->columnCount() < 3)
            SharedList2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        SharedList2->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        SharedList2->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        SharedList2->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        SharedList2->setObjectName(QString::fromUtf8("SharedList2"));
        SharedList2->setGeometry(QRect(930, 20, 331, 451));
        SharedList2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        SharedList2->setSelectionBehavior(QAbstractItemView::SelectRows);
        SharedList2->horizontalHeader()->setDefaultSectionSize(109);
        ServerList = new QTableWidget(tab_5);
        if (ServerList->columnCount() < 2)
            ServerList->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        ServerList->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        ServerList->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        ServerList->setObjectName(QString::fromUtf8("ServerList"));
        ServerList->setGeometry(QRect(80, 20, 401, 461));
        ServerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ServerList->setSelectionBehavior(QAbstractItemView::SelectRows);
        ServerList->setWordWrap(true);
        ServerList->horizontalHeader()->setVisible(false);
        ServerList->horizontalHeader()->setCascadingSectionResizes(true);
        ServerList->horizontalHeader()->setDefaultSectionSize(199);
        tabWidget_2->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        treeView = new QTreeView(tab_6);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(100, 60, 531, 391));
        pushButton_10 = new QPushButton(tab_6);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setGeometry(QRect(200, 490, 161, 71));
        listView_2 = new QListView(tab_6);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setGeometry(QRect(800, 40, 411, 331));
        pushButton = new QPushButton(tab_6);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(820, 480, 181, 81));
        pushButton_4 = new QPushButton(tab_6);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(1020, 520, 181, 41));
        pushButton_3 = new QPushButton(tab_6);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(820, 390, 181, 81));
        pushButton_21 = new QPushButton(tab_6);
        pushButton_21->setObjectName(QString::fromUtf8("pushButton_21"));
        pushButton_21->setGeometry(QRect(1020, 390, 181, 81));
        pushButton_8 = new QPushButton(tab_6);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(420, 490, 161, 71));
        widget = new QWidget(tab_6);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(1020, 480, 181, 29));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        horizontalLayout->addWidget(lineEdit_2);

        tabWidget_2->addTab(tab_6, QString());
        UserUI_1->setCentralWidget(centralwidget);
        menubar = new QMenuBar(UserUI_1);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1405, 25));
        menuMyDropBOx = new QMenu(menubar);
        menuMyDropBOx->setObjectName(QString::fromUtf8("menuMyDropBOx"));
        UserUI_1->setMenuBar(menubar);
        statusbar = new QStatusBar(UserUI_1);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        UserUI_1->setStatusBar(statusbar);

        menubar->addAction(menuMyDropBOx->menuAction());

        retranslateUi(UserUI_1);

        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(UserUI_1);
    } // setupUi

    void retranslateUi(QMainWindow *UserUI_1)
    {
        UserUI_1->setWindowTitle(QApplication::translate("UserUI_1", "MyDropBox", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("UserUI_1", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt; font-weight:600; color:#458981;\">TextLabel</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        pushButton_5->setWhatsThis(QApplication::translate("UserUI_1", "<html><head/><body><p>Sync All</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        pushButton_5->setText(QString());
        pushButton_6->setText(QString());
        commandLinkButton->setText(QApplication::translate("UserUI_1", "Read Me", 0, QApplication::UnicodeUTF8));
        pushButton_20->setText(QString());
        pushButton_13->setText(QApplication::translate("UserUI_1", "Download Selected", 0, QApplication::UnicodeUTF8));
        pushButton_19->setText(QApplication::translate("UserUI_1", "View Shared Folder", 0, QApplication::UnicodeUTF8));
        pushButton_16->setText(QApplication::translate("UserUI_1", "Transfer", 0, QApplication::UnicodeUTF8));
        pushButton_17->setText(QApplication::translate("UserUI_1", "Collaborate", 0, QApplication::UnicodeUTF8));
        pushButton_18->setText(QApplication::translate("UserUI_1", "Unshare", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("UserUI_1", "Change Priveledges", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("UserUI_1", "Delete Selected File", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("UserUI_1", "Share File", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = SharedList1->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("UserUI_1", "Filename", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = SharedList1->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("UserUI_1", "Shared With", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = SharedList1->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("UserUI_1", "Permission", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = SharedList2->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("UserUI_1", "Filename", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = SharedList2->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("UserUI_1", "Shared By", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = SharedList2->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("UserUI_1", "Permission", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = ServerList->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("UserUI_1", "Filename", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = ServerList->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("UserUI_1", "Path", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("UserUI_1", "Server", 0, QApplication::UnicodeUTF8));
        pushButton_10->setText(QApplication::translate("UserUI_1", "Open DropBox", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("UserUI_1", "Upload Selected Files", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("UserUI_1", "Upload Files In a Folder", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("UserUI_1", "Browse And Select Files", 0, QApplication::UnicodeUTF8));
        pushButton_21->setText(QApplication::translate("UserUI_1", "Deselect Files", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("UserUI_1", "Sync All", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("UserUI_1", "Folder  :", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_6), QApplication::translate("UserUI_1", "Local Storage", 0, QApplication::UnicodeUTF8));
        menuMyDropBOx->setTitle(QApplication::translate("UserUI_1", "MyDropBOx", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UserUI_1: public Ui_UserUI_1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERUI_1_H

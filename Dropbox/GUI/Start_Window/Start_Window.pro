#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T17:33:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Start_Window
TEMPLATE = app

LIBS+= -Wall -L/usr/lib -lssl -lcrypto


SOURCES += main.cpp\
        mainwindow.cpp \
    login_screen.cpp \
    register_screen.cpp\
    error_window.cpp\
    ../../Client/Client_register.cpp\
    ../../Client/Client_dropbox.cpp \
    ../../Client/Client_login.cpp \
    userui_1.cpp \
    progressbar.cpp \
    ../../Client/client_up_down.cpp \
    ../../Client/SSL_Client.cpp \
    sharesearch.cpp \
    readme.cpp

HEADERS  += mainwindow.h \
    login_screen.h \
    register_screen.h\
    ../../Client/Client_register.h \
    error_window.h\
    ../../Client/Client_dropbox.h \
    ../../Client/Client_login.h \
    userui_1.h \
    progressbar.h \
    ../../Client/client_up_down.h \
    ../../Client/SSL_Client.h \
    sharesearch.h \
    readme.h

FORMS    += mainwindow.ui \
    login_screen.ui \
    register_screen.ui \
    error_window.ui \
    userui_1.ui \
    progressbar.ui \
    sharesearch.ui \
    readme.ui \
    syncwin.ui

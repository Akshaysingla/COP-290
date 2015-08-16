/****************************************************************************
** Meta object code from reading C++ file 'userui_1.h'
**
** Created: Tue Mar 3 23:52:02 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Start_Window/userui_1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userui_1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UserUI_1[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      34,    9,    9,    9, 0x08,
      60,    9,    9,    9, 0x08,
      75,    9,    9,    9, 0x08,
      88,    9,    9,    9, 0x08,
     103,    9,    9,    9, 0x08,
     120,    9,    9,    9, 0x08,
     143,    9,    9,    9, 0x08,
     169,    9,    9,    9, 0x08,
     194,  191,    9,    9, 0x08,
     237,  191,    9,    9, 0x08,
     266,    9,    9,    9, 0x08,
     292,    9,    9,    9, 0x08,
     318,    9,    9,    9, 0x08,
     345,    9,    9,    9, 0x08,
     372,    9,    9,    9, 0x08,
     398,    9,    9,    9, 0x08,
     424,    9,    9,    9, 0x08,
     455,    9,    9,    9, 0x08,
     482,    9,    9,    9, 0x08,
     509,    9,    9,    9, 0x08,
     536,    9,    9,    9, 0x08,
     563,    9,    9,    9, 0x08,
     590,    9,    9,    9, 0x08,
     617,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UserUI_1[] = {
    "UserUI_1\0\0on_pushButton_clicked()\0"
    "on_pushButton_3_clicked()\0refresh_data()\0"
    "ServerList()\0SharedByList()\0"
    "SharedWithList()\0SyncDirectory(QString)\0"
    "SyncServerClient(QString)\0"
    "SendFileInfo(QString)\0,,\0"
    "ChangePriveledgeShare(QString,QString,int)\0"
    "Unshare(QString,QString,int)\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_20_clicked()\0"
    "on_pushButton_10_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_commandLinkButton_clicked()\0"
    "on_pushButton_16_clicked()\0"
    "on_pushButton_17_clicked()\0"
    "on_pushButton_18_clicked()\0"
    "on_pushButton_13_clicked()\0"
    "on_pushButton_19_clicked()\0"
    "on_pushButton_21_clicked()\0"
    "on_pushButton_4_clicked()\0"
};

void UserUI_1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UserUI_1 *_t = static_cast<UserUI_1 *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_3_clicked(); break;
        case 2: _t->refresh_data(); break;
        case 3: _t->ServerList(); break;
        case 4: _t->SharedByList(); break;
        case 5: _t->SharedWithList(); break;
        case 6: _t->SyncDirectory((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->SyncServerClient((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->SendFileInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->ChangePriveledgeShare((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->Unshare((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 11: _t->on_pushButton_5_clicked(); break;
        case 12: _t->on_pushButton_6_clicked(); break;
        case 13: _t->on_pushButton_20_clicked(); break;
        case 14: _t->on_pushButton_10_clicked(); break;
        case 15: _t->on_pushButton_9_clicked(); break;
        case 16: _t->on_pushButton_7_clicked(); break;
        case 17: _t->on_commandLinkButton_clicked(); break;
        case 18: _t->on_pushButton_16_clicked(); break;
        case 19: _t->on_pushButton_17_clicked(); break;
        case 20: _t->on_pushButton_18_clicked(); break;
        case 21: _t->on_pushButton_13_clicked(); break;
        case 22: _t->on_pushButton_19_clicked(); break;
        case 23: _t->on_pushButton_21_clicked(); break;
        case 24: _t->on_pushButton_4_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UserUI_1::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UserUI_1::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_UserUI_1,
      qt_meta_data_UserUI_1, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UserUI_1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UserUI_1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UserUI_1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UserUI_1))
        return static_cast<void*>(const_cast< UserUI_1*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int UserUI_1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

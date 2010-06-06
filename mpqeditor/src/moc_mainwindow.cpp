/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Jul 9 14:36:30 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mpqeditor__MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x08,
      33,   22,   22,   22, 0x08,
      44,   22,   22,   22, 0x08,
      61,   22,   22,   22, 0x08,
      73,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mpqeditor__MainWindow[] = {
    "mpqeditor::MainWindow\0\0fileNew()\0"
    "fileOpen()\0fileOpenRecent()\0fileClose()\0"
    "editAdd()\0"
};

const QMetaObject mpqeditor::MainWindow::staticMetaObject = {
    { &KMainWindow::staticMetaObject, qt_meta_stringdata_mpqeditor__MainWindow,
      qt_meta_data_mpqeditor__MainWindow, 0 }
};

const QMetaObject *mpqeditor::MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *mpqeditor::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mpqeditor__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return KMainWindow::qt_metacast(_clname);
}

int mpqeditor::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: fileNew(); break;
        case 1: fileOpen(); break;
        case 2: fileOpenRecent(); break;
        case 3: fileClose(); break;
        case 4: editAdd(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'plugin.h'
**
** Created: Sun May 31 14:32:59 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "plugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_katevjasside__Plugin[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_katevjasside__Plugin[] = {
    "katevjasside::Plugin\0"
};

const QMetaObject katevjasside::Plugin::staticMetaObject = {
    { &Kate::Plugin::staticMetaObject, qt_meta_stringdata_katevjasside__Plugin,
      qt_meta_data_katevjasside__Plugin, 0 }
};

const QMetaObject *katevjasside::Plugin::metaObject() const
{
    return &staticMetaObject;
}

void *katevjasside::Plugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_katevjasside__Plugin))
        return static_cast<void*>(const_cast< Plugin*>(this));
    if (!strcmp(_clname, "Kate::PluginConfigPageInterface"))
        return static_cast< Kate::PluginConfigPageInterface*>(const_cast< Plugin*>(this));
    if (!strcmp(_clname, "org.kde.Kate.PluginConfigPageInterface"))
        return static_cast< Kate::PluginConfigPageInterface*>(const_cast< Plugin*>(this));
    typedef Kate::Plugin QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int katevjasside::Plugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Kate::Plugin QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

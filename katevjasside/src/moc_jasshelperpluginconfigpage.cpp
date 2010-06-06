/****************************************************************************
** Meta object code from reading C++ file 'jasshelperpluginconfigpage.h'
**
** Created: Fri Aug 14 21:40:24 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "jasshelperpluginconfigpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jasshelperpluginconfigpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_katevjasside__JasshelperPluginConfigPage[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      42,   41,   41,   41, 0x0a,
      50,   41,   41,   41, 0x0a,
      61,   41,   41,   41, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_katevjasside__JasshelperPluginConfigPage[] = {
    "katevjasside::JasshelperPluginConfigPage\0"
    "\0apply()\0defaults()\0reset()\0"
};

const QMetaObject katevjasside::JasshelperPluginConfigPage::staticMetaObject = {
    { &Kate::PluginConfigPage::staticMetaObject, qt_meta_stringdata_katevjasside__JasshelperPluginConfigPage,
      qt_meta_data_katevjasside__JasshelperPluginConfigPage, 0 }
};

const QMetaObject *katevjasside::JasshelperPluginConfigPage::metaObject() const
{
    return &staticMetaObject;
}

void *katevjasside::JasshelperPluginConfigPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_katevjasside__JasshelperPluginConfigPage))
        return static_cast<void*>(const_cast< JasshelperPluginConfigPage*>(this));
    if (!strcmp(_clname, "Ui::JasshelperConfigPageWidget"))
        return static_cast< Ui::JasshelperConfigPageWidget*>(const_cast< JasshelperPluginConfigPage*>(this));
    typedef Kate::PluginConfigPage QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int katevjasside::JasshelperPluginConfigPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef Kate::PluginConfigPage QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: apply(); break;
        case 1: defaults(); break;
        case 2: reset(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

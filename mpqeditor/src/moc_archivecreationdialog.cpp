/****************************************************************************
** Meta object code from reading C++ file 'archivecreationdialog.h'
**
** Created: Mon Jun 29 14:44:17 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "archivecreationdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'archivecreationdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_mpqeditor__ArchiveCreationDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      34,   33,   33,   33, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_mpqeditor__ArchiveCreationDialog[] = {
    "mpqeditor::ArchiveCreationDialog\0\0"
    "createArchive()\0"
};

const QMetaObject mpqeditor::ArchiveCreationDialog::staticMetaObject = {
    { &KDialog::staticMetaObject, qt_meta_stringdata_mpqeditor__ArchiveCreationDialog,
      qt_meta_data_mpqeditor__ArchiveCreationDialog, 0 }
};

const QMetaObject *mpqeditor::ArchiveCreationDialog::metaObject() const
{
    return &staticMetaObject;
}

void *mpqeditor::ArchiveCreationDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_mpqeditor__ArchiveCreationDialog))
        return static_cast<void*>(const_cast< ArchiveCreationDialog*>(this));
    if (!strcmp(_clname, "Ui::ArchiveCreationDialog"))
        return static_cast< Ui::ArchiveCreationDialog*>(const_cast< ArchiveCreationDialog*>(this));
    return KDialog::qt_metacast(_clname);
}

int mpqeditor::ArchiveCreationDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createArchive(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

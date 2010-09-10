/****************************************************************************
** Meta object code from reading C++ file 'editor.hpp'
**
** Created: Fri Sep 10 08:43:03 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "editor.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wc3lib__editor__Editor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      33,   23,   23,   23, 0x0a,
      53,   23,   23,   23, 0x0a,
      73,   23,   23,   23, 0x0a,
      91,   23,   23,   23, 0x0a,
     110,   23,   23,   23, 0x0a,
     131,   23,   23,   23, 0x0a,
     146,   23,   23,   23, 0x0a,
     166,   23,   23,   23, 0x0a,
     186,   23,   23,   23, 0x0a,
     202,   23,   23,   23, 0x0a,
     220,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_wc3lib__editor__Editor[] = {
    "wc3lib::editor::Editor\0\0newMap()\0"
    "showTerrainEditor()\0showTriggerEditor()\0"
    "showSoundEditor()\0showObjectEditor()\0"
    "showCampaignEditor()\0showAiEditor()\0"
    "showObjectManager()\0showImportManager()\0"
    "showMpqEditor()\0showModelEditor()\0"
    "showTextureEditor()\0"
};

const QMetaObject wc3lib::editor::Editor::staticMetaObject = {
    { &KMainWindow::staticMetaObject, qt_meta_stringdata_wc3lib__editor__Editor,
      qt_meta_data_wc3lib__editor__Editor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wc3lib::editor::Editor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wc3lib::editor::Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wc3lib::editor::Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wc3lib__editor__Editor))
        return static_cast<void*>(const_cast< Editor*>(this));
    return KMainWindow::qt_metacast(_clname);
}

int wc3lib::editor::Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newMap(); break;
        case 1: showTerrainEditor(); break;
        case 2: showTriggerEditor(); break;
        case 3: showSoundEditor(); break;
        case 4: showObjectEditor(); break;
        case 5: showCampaignEditor(); break;
        case 6: showAiEditor(); break;
        case 7: showObjectManager(); break;
        case 8: showImportManager(); break;
        case 9: showMpqEditor(); break;
        case 10: showModelEditor(); break;
        case 11: showTextureEditor(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

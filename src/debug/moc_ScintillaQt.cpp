/****************************************************************************
** Meta object code from reading C++ file 'ScintillaQt.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QScintilla/src/ScintillaQt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScintillaQt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QsciScintillaQt_t {
    uint offsetsAndSizes[8];
    char stringdata0[16];
    char stringdata1[7];
    char stringdata2[1];
    char stringdata3[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciScintillaQt_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciScintillaQt_t qt_meta_stringdata_QsciScintillaQt = {
    {
        QT_MOC_LITERAL(0, 15),  // "QsciScintillaQt"
        QT_MOC_LITERAL(16, 6),  // "onIdle"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 18)   // "onSelectionChanged"
    },
    "QsciScintillaQt",
    "onIdle",
    "",
    "onSelectionChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciScintillaQt[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   26,    2, 0x08,    1 /* Private */,
       3,    0,   27,    2, 0x08,    2 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QsciScintillaQt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QsciScintillaQt *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onIdle(); break;
        case 1: _t->onSelectionChanged(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject QsciScintillaQt::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QsciScintillaQt.offsetsAndSizes,
    qt_meta_data_QsciScintillaQt,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciScintillaQt_t
, QtPrivate::TypeAndForceComplete<QsciScintillaQt, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *QsciScintillaQt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciScintillaQt::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciScintillaQt.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Scintilla::ScintillaBase"))
        return static_cast< Scintilla::ScintillaBase*>(this);
    return QObject::qt_metacast(_clname);
}

int QsciScintillaQt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

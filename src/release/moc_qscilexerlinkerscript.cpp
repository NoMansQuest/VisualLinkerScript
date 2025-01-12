/****************************************************************************
** Meta object code from reading C++ file 'qscilexerlinkerscript.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QScintilla/src/Qsci/qscilexerlinkerscript.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexerlinkerscript.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QsciLexerLinkerScript_t {
    uint offsetsAndSizes[8];
    char stringdata0[22];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciLexerLinkerScript_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciLexerLinkerScript_t qt_meta_stringdata_QsciLexerLinkerScript = {
    {
        QT_MOC_LITERAL(0, 21),  // "QsciLexerLinkerScript"
        QT_MOC_LITERAL(22, 13),  // "setFoldAtElse"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 4)   // "fold"
    },
    "QsciLexerLinkerScript",
    "setFoldAtElse",
    "",
    "fold"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciLexerLinkerScript[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x0a,    1 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void QsciLexerLinkerScript::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QsciLexerLinkerScript *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setFoldAtElse((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QsciLexerLinkerScript::staticMetaObject = { {
    QMetaObject::SuperData::link<QsciLexer::staticMetaObject>(),
    qt_meta_stringdata_QsciLexerLinkerScript.offsetsAndSizes,
    qt_meta_data_QsciLexerLinkerScript,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciLexerLinkerScript_t
, QtPrivate::TypeAndForceComplete<QsciLexerLinkerScript, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *QsciLexerLinkerScript::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciLexerLinkerScript::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerLinkerScript.stringdata0))
        return static_cast<void*>(this);
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerLinkerScript::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

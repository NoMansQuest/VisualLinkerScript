/****************************************************************************
** Meta object code from reading C++ file 'qscimacro.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QScintilla/src/Qsci/qscimacro.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscimacro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QsciMacro_t {
    uint offsetsAndSizes[18];
    char stringdata0[10];
    char stringdata1[5];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[13];
    char stringdata5[7];
    char stringdata6[4];
    char stringdata7[7];
    char stringdata8[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciMacro_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciMacro_t qt_meta_stringdata_QsciMacro = {
    {
        QT_MOC_LITERAL(0, 9),  // "QsciMacro"
        QT_MOC_LITERAL(10, 4),  // "play"
        QT_MOC_LITERAL(15, 0),  // ""
        QT_MOC_LITERAL(16, 14),  // "startRecording"
        QT_MOC_LITERAL(31, 12),  // "endRecording"
        QT_MOC_LITERAL(44, 6),  // "record"
        QT_MOC_LITERAL(51, 3),  // "msg"
        QT_MOC_LITERAL(55, 6),  // "wParam"
        QT_MOC_LITERAL(62, 6)   // "lParam"
    },
    "QsciMacro",
    "play",
    "",
    "startRecording",
    "endRecording",
    "record",
    "msg",
    "wParam",
    "lParam"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciMacro[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x0a,    1 /* Public */,
       3,    0,   39,    2, 0x0a,    2 /* Public */,
       4,    0,   40,    2, 0x0a,    3 /* Public */,
       5,    3,   41,    2, 0x08,    4 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::ULong, QMetaType::VoidStar,    6,    7,    8,

       0        // eod
};

void QsciMacro::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QsciMacro *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->play(); break;
        case 1: _t->startRecording(); break;
        case 2: _t->endRecording(); break;
        case 3: _t->record((*reinterpret_cast< std::add_pointer_t<uint>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ulong>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<void*>>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject QsciMacro::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QsciMacro.offsetsAndSizes,
    qt_meta_data_QsciMacro,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciMacro_t
, QtPrivate::TypeAndForceComplete<QsciMacro, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<unsigned int, std::false_type>, QtPrivate::TypeAndForceComplete<unsigned long, std::false_type>, QtPrivate::TypeAndForceComplete<void *, std::false_type>


>,
    nullptr
} };


const QMetaObject *QsciMacro::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciMacro::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciMacro.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QsciMacro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

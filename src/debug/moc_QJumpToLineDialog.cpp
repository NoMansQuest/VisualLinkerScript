/****************************************************************************
** Meta object code from reading C++ file 'QJumpToLineDialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/InputDialogs/QJumpToLineDialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QJumpToLineDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QJumpToLineDialog_t {
    uint offsetsAndSizes[14];
    char stringdata0[18];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[11];
    char stringdata5[15];
    char stringdata6[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QJumpToLineDialog_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QJumpToLineDialog_t qt_meta_stringdata_QJumpToLineDialog = {
    {
        QT_MOC_LITERAL(0, 17),  // "QJumpToLineDialog"
        QT_MOC_LITERAL(18, 16),  // "evNavigateToLine"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 8),  // "uint32_t"
        QT_MOC_LITERAL(45, 10),  // "lineNumber"
        QT_MOC_LITERAL(56, 14),  // "BuildInterface"
        QT_MOC_LITERAL(71, 23)   // "onLineEditReturnPressed"
    },
    "QJumpToLineDialog",
    "evNavigateToLine",
    "",
    "uint32_t",
    "lineNumber",
    "BuildInterface",
    "onLineEditReturnPressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QJumpToLineDialog[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   35,    2, 0x08,    3 /* Private */,
       6,    0,   36,    2, 0x08,    4 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QJumpToLineDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QJumpToLineDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->evNavigateToLine((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        case 1: _t->BuildInterface(); break;
        case 2: _t->onLineEditReturnPressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QJumpToLineDialog::*)(uint32_t );
            if (_t _q_method = &QJumpToLineDialog::evNavigateToLine; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject QJumpToLineDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_QJumpToLineDialog.offsetsAndSizes,
    qt_meta_data_QJumpToLineDialog,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QJumpToLineDialog_t
, QtPrivate::TypeAndForceComplete<QJumpToLineDialog, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *QJumpToLineDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QJumpToLineDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QJumpToLineDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QJumpToLineDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QJumpToLineDialog::evNavigateToLine(uint32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

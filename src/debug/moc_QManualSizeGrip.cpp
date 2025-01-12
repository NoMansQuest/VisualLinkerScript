/****************************************************************************
** Meta object code from reading C++ file 'QManualSizeGrip.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QManualSizeGrip/QManualSizeGrip.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QManualSizeGrip.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QManualSizeGrip_t {
    uint offsetsAndSizes[16];
    char stringdata0[16];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[3];
    char stringdata5[3];
    char stringdata6[14];
    char stringdata7[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QManualSizeGrip_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QManualSizeGrip_t qt_meta_stringdata_QManualSizeGrip = {
    {
        QT_MOC_LITERAL(0, 15),  // "QManualSizeGrip"
        QT_MOC_LITERAL(16, 7),  // "resized"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 14),  // "globalPosition"
        QT_MOC_LITERAL(40, 2),  // "dx"
        QT_MOC_LITERAL(43, 2),  // "dy"
        QT_MOC_LITERAL(46, 13),  // "resizeStarted"
        QT_MOC_LITERAL(60, 14)   // "resizeFinished"
    },
    "QManualSizeGrip",
    "resized",
    "",
    "globalPosition",
    "dx",
    "dy",
    "resizeStarted",
    "resizeFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QManualSizeGrip[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   32,    2, 0x06,    1 /* Public */,
       6,    1,   39,    2, 0x06,    5 /* Public */,
       7,    0,   42,    2, 0x06,    7 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QPointF,    3,
    QMetaType::Void,

       0        // eod
};

void QManualSizeGrip::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QManualSizeGrip *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->resized((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 1: _t->resizeStarted((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 2: _t->resizeFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QManualSizeGrip::*)(QPointF , int , int );
            if (_t _q_method = &QManualSizeGrip::resized; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QManualSizeGrip::*)(QPointF );
            if (_t _q_method = &QManualSizeGrip::resizeStarted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QManualSizeGrip::*)();
            if (_t _q_method = &QManualSizeGrip::resizeFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QManualSizeGrip::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_QManualSizeGrip.offsetsAndSizes,
    qt_meta_data_QManualSizeGrip,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QManualSizeGrip_t
, QtPrivate::TypeAndForceComplete<QManualSizeGrip, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QPointF, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QPointF, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>



>,
    nullptr
} };


const QMetaObject *QManualSizeGrip::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QManualSizeGrip::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QManualSizeGrip.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int QManualSizeGrip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
void QManualSizeGrip::resized(QPointF _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QManualSizeGrip::resizeStarted(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QManualSizeGrip::resizeFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

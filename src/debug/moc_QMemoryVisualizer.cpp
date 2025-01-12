/****************************************************************************
** Meta object code from reading C++ file 'QMemoryVisualizer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/MemoryVisualizer/QMemoryVisualizer.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMemoryVisualizer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QMemoryVisualizer_t {
    uint offsetsAndSizes[24];
    char stringdata0[18];
    char stringdata1[27];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[7];
    char stringdata5[26];
    char stringdata6[20];
    char stringdata7[23];
    char stringdata8[22];
    char stringdata9[22];
    char stringdata10[16];
    char stringdata11[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QMemoryVisualizer_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QMemoryVisualizer_t qt_meta_stringdata_QMemoryVisualizer = {
    {
        QT_MOC_LITERAL(0, 17),  // "QMemoryVisualizer"
        QT_MOC_LITERAL(18, 26),  // "OnScrollChangeByMouseWheel"
        QT_MOC_LITERAL(45, 0),  // ""
        QT_MOC_LITERAL(46, 6),  // "xSteps"
        QT_MOC_LITERAL(53, 6),  // "ySteps"
        QT_MOC_LITERAL(60, 25),  // "OnZoomChangedByMouseWheel"
        QT_MOC_LITERAL(86, 19),  // "zoomChangeInPercent"
        QT_MOC_LITERAL(106, 22),  // "OnScrollPositionChange"
        QT_MOC_LITERAL(129, 21),  // "OnZoomIncreaseClicked"
        QT_MOC_LITERAL(151, 21),  // "OnZoomDecreaseClicked"
        QT_MOC_LITERAL(173, 15),  // "OnZoomToContent"
        QT_MOC_LITERAL(189, 13)   // "OnZoomUpdated"
    },
    "QMemoryVisualizer",
    "OnScrollChangeByMouseWheel",
    "",
    "xSteps",
    "ySteps",
    "OnZoomChangedByMouseWheel",
    "zoomChangeInPercent",
    "OnScrollPositionChange",
    "OnZoomIncreaseClicked",
    "OnZoomDecreaseClicked",
    "OnZoomToContent",
    "OnZoomUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMemoryVisualizer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   56,    2, 0x108,    1 /* Private | MethodIsConst  */,
       5,    1,   61,    2, 0x108,    4 /* Private | MethodIsConst  */,
       7,    0,   64,    2, 0x108,    6 /* Private | MethodIsConst  */,
       8,    0,   65,    2, 0x108,    7 /* Private | MethodIsConst  */,
       9,    0,   66,    2, 0x108,    8 /* Private | MethodIsConst  */,
      10,    0,   67,    2, 0x108,    9 /* Private | MethodIsConst  */,
      11,    0,   68,    2, 0x108,   10 /* Private | MethodIsConst  */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QMemoryVisualizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QMemoryVisualizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->OnScrollChangeByMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->OnZoomChangedByMouseWheel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->OnScrollPositionChange(); break;
        case 3: _t->OnZoomIncreaseClicked(); break;
        case 4: _t->OnZoomDecreaseClicked(); break;
        case 5: _t->OnZoomToContent(); break;
        case 6: _t->OnZoomUpdated(); break;
        default: ;
        }
    }
}

const QMetaObject QMemoryVisualizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_QMemoryVisualizer.offsetsAndSizes,
    qt_meta_data_QMemoryVisualizer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QMemoryVisualizer_t
, QtPrivate::TypeAndForceComplete<QMemoryVisualizer, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *QMemoryVisualizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMemoryVisualizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QMemoryVisualizer.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int QMemoryVisualizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'SciClasses.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QScintilla/src/SciClasses.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SciClasses.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QsciSciCallTip_t {
    uint offsetsAndSizes[2];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciSciCallTip_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciSciCallTip_t qt_meta_stringdata_QsciSciCallTip = {
    {
        QT_MOC_LITERAL(0, 14)   // "QsciSciCallTip"
    },
    "QsciSciCallTip"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciSciCallTip[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QsciSciCallTip::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject QsciSciCallTip::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_QsciSciCallTip.offsetsAndSizes,
    qt_meta_data_QsciSciCallTip,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciSciCallTip_t
, QtPrivate::TypeAndForceComplete<QsciSciCallTip, std::true_type>



>,
    nullptr
} };


const QMetaObject *QsciSciCallTip::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciSciCallTip::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciSciCallTip.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QsciSciCallTip::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QsciSciPopup_t {
    uint offsetsAndSizes[8];
    char stringdata0[13];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[4];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciSciPopup_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciSciPopup_t qt_meta_stringdata_QsciSciPopup = {
    {
        QT_MOC_LITERAL(0, 12),  // "QsciSciPopup"
        QT_MOC_LITERAL(13, 12),  // "on_triggered"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 3)   // "cmd"
    },
    "QsciSciPopup",
    "on_triggered",
    "",
    "cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciSciPopup[] = {

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
       1,    1,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void QsciSciPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QsciSciPopup *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_triggered((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QsciSciPopup::staticMetaObject = { {
    QMetaObject::SuperData::link<QMenu::staticMetaObject>(),
    qt_meta_stringdata_QsciSciPopup.offsetsAndSizes,
    qt_meta_data_QsciSciPopup,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciSciPopup_t
, QtPrivate::TypeAndForceComplete<QsciSciPopup, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *QsciSciPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciSciPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciSciPopup.stringdata0))
        return static_cast<void*>(this);
    return QMenu::qt_metacast(_clname);
}

int QsciSciPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QsciSciListBox_t {
    uint offsetsAndSizes[2];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QsciSciListBox_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QsciSciListBox_t qt_meta_stringdata_QsciSciListBox = {
    {
        QT_MOC_LITERAL(0, 14)   // "QsciSciListBox"
    },
    "QsciSciListBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QsciSciListBox[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QsciSciListBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject QsciSciListBox::staticMetaObject = { {
    QMetaObject::SuperData::link<QListWidget::staticMetaObject>(),
    qt_meta_stringdata_QsciSciListBox.offsetsAndSizes,
    qt_meta_data_QsciSciListBox,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QsciSciListBox_t
, QtPrivate::TypeAndForceComplete<QsciSciListBox, std::true_type>



>,
    nullptr
} };


const QMetaObject *QsciSciListBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QsciSciListBox::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QsciSciListBox.stringdata0))
        return static_cast<void*>(this);
    return QListWidget::qt_metacast(_clname);
}

int QsciSciListBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

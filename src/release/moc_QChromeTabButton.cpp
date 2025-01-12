/****************************************************************************
** Meta object code from reading C++ file 'QChromeTabButton.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QChromeTab/QChromeTabButton.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QChromeTabButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QChromeTabButton_t {
    uint offsetsAndSizes[28];
    char stringdata0[17];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[6];
    char stringdata5[23];
    char stringdata6[24];
    char stringdata7[22];
    char stringdata8[19];
    char stringdata9[15];
    char stringdata10[14];
    char stringdata11[12];
    char stringdata12[11];
    char stringdata13[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QChromeTabButton_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QChromeTabButton_t qt_meta_stringdata_QChromeTabButton = {
    {
        QT_MOC_LITERAL(0, 16),  // "QChromeTabButton"
        QT_MOC_LITERAL(17, 25),  // "evUserRequestedActivation"
        QT_MOC_LITERAL(43, 0),  // ""
        QT_MOC_LITERAL(44, 8),  // "uint32_t"
        QT_MOC_LITERAL(53, 5),  // "tabId"
        QT_MOC_LITERAL(59, 22),  // "evUserRequestedClosure"
        QT_MOC_LITERAL(82, 23),  // "evActiveTabStateChanged"
        QT_MOC_LITERAL(106, 21),  // "evDisplayTitleChanged"
        QT_MOC_LITERAL(128, 18),  // "CloseButtonPressed"
        QT_MOC_LITERAL(147, 14),  // "RefreshStyling"
        QT_MOC_LITERAL(162, 13),  // "IsHighlighted"
        QT_MOC_LITERAL(176, 11),  // "IsActiveTab"
        QT_MOC_LITERAL(188, 10),  // "IsHovering"
        QT_MOC_LITERAL(199, 12)   // "DisplayTitle"
    },
    "QChromeTabButton",
    "evUserRequestedActivation",
    "",
    "uint32_t",
    "tabId",
    "evUserRequestedClosure",
    "evActiveTabStateChanged",
    "evDisplayTitleChanged",
    "CloseButtonPressed",
    "RefreshStyling",
    "IsHighlighted",
    "IsActiveTab",
    "IsHovering",
    "DisplayTitle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QChromeTabButton[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   60, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    5 /* Public */,
       5,    1,   53,    2, 0x06,    7 /* Public */,
       6,    0,   56,    2, 0x06,    9 /* Public */,
       7,    0,   57,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   58,    2, 0x09,   11 /* Protected */,
       9,    0,   59,    2, 0x09,   12 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      10, QMetaType::Bool, 0x00115001, uint(-1), 0,
      11, QMetaType::Bool, 0x00115001, uint(2), 0,
      12, QMetaType::Bool, 0x00115001, uint(-1), 0,
      13, QMetaType::QString, 0x00115003, uint(3), 0,

       0        // eod
};

void QChromeTabButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QChromeTabButton *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->evUserRequestedActivation((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        case 1: _t->evUserRequestedClosure((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        case 2: _t->evActiveTabStateChanged(); break;
        case 3: _t->evDisplayTitleChanged(); break;
        case 4: _t->CloseButtonPressed(); break;
        case 5: _t->RefreshStyling(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QChromeTabButton::*)(uint32_t );
            if (_t _q_method = &QChromeTabButton::evUserRequestedActivation; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QChromeTabButton::*)(uint32_t );
            if (_t _q_method = &QChromeTabButton::evUserRequestedClosure; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QChromeTabButton::*)();
            if (_t _q_method = &QChromeTabButton::evActiveTabStateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QChromeTabButton::*)();
            if (_t _q_method = &QChromeTabButton::evDisplayTitleChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<QChromeTabButton *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->IsHighlighted(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->IsActiveTab(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->IsHovering(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->DisplayTitle(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<QChromeTabButton *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->SetDisplayTitle(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QChromeTabButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_QChromeTabButton.offsetsAndSizes,
    qt_meta_data_QChromeTabButton,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QChromeTabButton_t
, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<bool, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QChromeTabButton, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *QChromeTabButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QChromeTabButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QChromeTabButton.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int QChromeTabButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QChromeTabButton::evUserRequestedActivation(uint32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QChromeTabButton::evUserRequestedClosure(uint32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QChromeTabButton::evActiveTabStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QChromeTabButton::evDisplayTitleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

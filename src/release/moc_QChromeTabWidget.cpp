/****************************************************************************
** Meta object code from reading C++ file 'QChromeTabWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Components/QChromeTab/QChromeTabWidget.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QChromeTabWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QChromeTabWidget_t {
    uint offsetsAndSizes[24];
    char stringdata0[17];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[24];
    char stringdata4[10];
    char stringdata5[26];
    char stringdata6[9];
    char stringdata7[20];
    char stringdata8[24];
    char stringdata9[19];
    char stringdata10[6];
    char stringdata11[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_QChromeTabWidget_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_QChromeTabWidget_t qt_meta_stringdata_QChromeTabWidget = {
    {
        QT_MOC_LITERAL(0, 16),  // "QChromeTabWidget"
        QT_MOC_LITERAL(17, 18),  // "evActiveTabChanged"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 23),  // "std::optional<uint32_t>"
        QT_MOC_LITERAL(61, 9),  // "activeTab"
        QT_MOC_LITERAL(71, 25),  // "evUserRequestedTabClosure"
        QT_MOC_LITERAL(97, 8),  // "uint32_t"
        QT_MOC_LITERAL(106, 19),  // "requestedTabToClose"
        QT_MOC_LITERAL(126, 23),  // "evTabsCollectionUpdated"
        QT_MOC_LITERAL(150, 18),  // "CloseButtonClicked"
        QT_MOC_LITERAL(169, 5),  // "tabId"
        QT_MOC_LITERAL(175, 22)   // "TabRequestedActivation"
    },
    "QChromeTabWidget",
    "evActiveTabChanged",
    "",
    "std::optional<uint32_t>",
    "activeTab",
    "evUserRequestedTabClosure",
    "uint32_t",
    "requestedTabToClose",
    "evTabsCollectionUpdated",
    "CloseButtonClicked",
    "tabId",
    "TabRequestedActivation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QChromeTabWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       8,    0,   50,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    1,   51,    2, 0x09,    6 /* Protected */,
      11,    1,   54,    2, 0x09,    8 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,   10,
    QMetaType::Void, 0x80000000 | 6,   10,

       0        // eod
};

void QChromeTabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QChromeTabWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->evActiveTabChanged((*reinterpret_cast< std::add_pointer_t<std::optional<uint32_t>>>(_a[1]))); break;
        case 1: _t->evUserRequestedTabClosure((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        case 2: _t->evTabsCollectionUpdated(); break;
        case 3: _t->CloseButtonClicked((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        case 4: _t->TabRequestedActivation((*reinterpret_cast< std::add_pointer_t<uint32_t>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QChromeTabWidget::*)(std::optional<uint32_t> );
            if (_t _q_method = &QChromeTabWidget::evActiveTabChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QChromeTabWidget::*)(uint32_t );
            if (_t _q_method = &QChromeTabWidget::evUserRequestedTabClosure; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QChromeTabWidget::*)();
            if (_t _q_method = &QChromeTabWidget::evTabsCollectionUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QChromeTabWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_QChromeTabWidget.offsetsAndSizes,
    qt_meta_data_QChromeTabWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_QChromeTabWidget_t
, QtPrivate::TypeAndForceComplete<QChromeTabWidget, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::optional<uint32_t>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint32_t, std::false_type>


>,
    nullptr
} };


const QMetaObject *QChromeTabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QChromeTabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QChromeTabWidget.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int QChromeTabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QChromeTabWidget::evActiveTabChanged(std::optional<uint32_t> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QChromeTabWidget::evUserRequestedTabClosure(uint32_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QChromeTabWidget::evTabsCollectionUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

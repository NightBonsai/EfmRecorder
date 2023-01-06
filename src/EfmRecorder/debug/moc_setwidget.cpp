/****************************************************************************
** Meta object code from reading C++ file 'setwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/setwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SetWidget_t {
    QByteArrayData data[8];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SetWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SetWidget_t qt_meta_stringdata_SetWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SetWidget"
QT_MOC_LITERAL(1, 10, 7), // "setInfo"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 15), // "selectVideoPath"
QT_MOC_LITERAL(4, 35, 15), // "selectImagePath"
QT_MOC_LITERAL(5, 51, 8), // "setJudge"
QT_MOC_LITERAL(6, 60, 8), // "renewSet"
QT_MOC_LITERAL(7, 69, 7) // "showSet"

    },
    "SetWidget\0setInfo\0\0selectVideoPath\0"
    "selectImagePath\0setJudge\0renewSet\0"
    "showSet"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SetWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SetWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SetWidget *_t = static_cast<SetWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setInfo(); break;
        case 1: _t->selectVideoPath(); break;
        case 2: _t->selectImagePath(); break;
        case 3: _t->setJudge(); break;
        case 4: _t->renewSet(); break;
        case 5: _t->showSet(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SetWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SetWidget.data,
      qt_meta_data_SetWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SetWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SetWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SetWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SetWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/mainwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWidget_t {
    QByteArrayData data[15];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWidget_t qt_meta_stringdata_MainWidget = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWidget"
QT_MOC_LITERAL(1, 11, 14), // "toSetWinSIGNAL"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "receiveImg"
QT_MOC_LITERAL(4, 38, 5), // "image"
QT_MOC_LITERAL(5, 44, 8), // "toLogWin"
QT_MOC_LITERAL(6, 53, 10), // "toImageWin"
QT_MOC_LITERAL(7, 64, 10), // "toVideoWin"
QT_MOC_LITERAL(8, 75, 14), // "toTranscodeWin"
QT_MOC_LITERAL(9, 90, 8), // "toSetWin"
QT_MOC_LITERAL(10, 99, 10), // "toDailyWin"
QT_MOC_LITERAL(11, 110, 8), // "showMain"
QT_MOC_LITERAL(12, 119, 15), // "initLoginPolice"
QT_MOC_LITERAL(13, 135, 15), // "catchImageJudge"
QT_MOC_LITERAL(14, 151, 3) // "end"

    },
    "MainWidget\0toSetWinSIGNAL\0\0receiveImg\0"
    "image\0toLogWin\0toImageWin\0toVideoWin\0"
    "toTranscodeWin\0toSetWin\0toDailyWin\0"
    "showMain\0initLoginPolice\0catchImageJudge\0"
    "end"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   75,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x0a /* Public */,
       6,    0,   79,    2, 0x0a /* Public */,
       7,    0,   80,    2, 0x0a /* Public */,
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      13,    0,   86,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QImage,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWidget *_t = static_cast<MainWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toSetWinSIGNAL(); break;
        case 1: _t->receiveImg((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->toLogWin(); break;
        case 3: _t->toImageWin(); break;
        case 4: _t->toVideoWin(); break;
        case 5: _t->toTranscodeWin(); break;
        case 6: _t->toSetWin(); break;
        case 7: _t->toDailyWin(); break;
        case 8: _t->showMain(); break;
        case 9: _t->initLoginPolice(); break;
        case 10: _t->catchImageJudge(); break;
        case 11: _t->end(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWidget::toSetWinSIGNAL)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWidget.data,
      qt_meta_data_MainWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MainWidget::toSetWinSIGNAL()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

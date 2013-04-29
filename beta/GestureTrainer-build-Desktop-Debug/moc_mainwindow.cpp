/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Apr 28 20:21:02 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GestureTrainer/forms/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      62,   11,   11,   11, 0x08,
      77,   11,   11,   11, 0x08,
      91,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     137,  131,   11,   11, 0x08,
     152,  131,   11,   11, 0x08,
     167,  131,   11,   11, 0x08,
     184,  131,   11,   11, 0x08,
     199,  131,   11,   11, 0x08,
     214,  131,   11,   11, 0x08,
     237,  231,   11,   11, 0x08,
     271,  231,   11,   11, 0x08,
     304,  231,   11,   11, 0x08,
     338,  231,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0index\0on_tabWidget_currentChanged(int)\0"
    "setImage()\0toggleCamera()\0updateTimer()\0"
    "processColorDetection()\0showHistogram()\0"
    "value\0setMinHue(int)\0setMinSat(int)\0"
    "setMinValue(int)\0setMaxHue(int)\0"
    "setMaxSat(int)\0setMaxValue(int)\0state\0"
    "on_check_Invert_stateChanged(int)\0"
    "on_check_Erode_stateChanged(int)\0"
    "on_check_Dilate_stateChanged(int)\0"
    "on_check_Blur_stateChanged(int)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setImage(); break;
        case 2: _t->toggleCamera(); break;
        case 3: _t->updateTimer(); break;
        case 4: _t->processColorDetection(); break;
        case 5: _t->showHistogram(); break;
        case 6: _t->setMinHue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setMinSat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setMinValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->setMaxHue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setMaxSat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setMaxValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_check_Invert_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_check_Erode_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_check_Dilate_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_check_Blur_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'wdgprintlabels.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wdgprintlabels.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgprintlabels.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wdgPrintLabels[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      40,   15,   15,   15, 0x08,
      67,   63,   15,   15, 0x08,
      97,   15,   15,   15, 0x08,
     112,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wdgPrintLabels[] = {
    "wdgPrintLabels\0\0reloadPrintParameters()\0"
    "spinBoxValuesChanged()\0str\0"
    "comboBoxIndexChanged(QString)\0"
    "processPrint()\0updatePrintButton()\0"
};

void wdgPrintLabels::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        wdgPrintLabels *_t = static_cast<wdgPrintLabels *>(_o);
        switch (_id) {
        case 0: _t->reloadPrintParameters(); break;
        case 1: _t->spinBoxValuesChanged(); break;
        case 2: _t->comboBoxIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->processPrint(); break;
        case 4: _t->updatePrintButton(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData wdgPrintLabels::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wdgPrintLabels::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgPrintLabels,
      qt_meta_data_wdgPrintLabels, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wdgPrintLabels::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wdgPrintLabels::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wdgPrintLabels::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wdgPrintLabels))
        return static_cast<void*>(const_cast< wdgPrintLabels*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgPrintLabels::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

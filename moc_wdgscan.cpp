/****************************************************************************
** Meta object code from reading C++ file 'wdgscan.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wdgscan.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgscan.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wdgScan[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   36,   31,    8, 0x0a,
      76,    8,   31,    8, 0x0a,
      96,    8,    8,    8, 0x0a,
     135,  118,    8,    8, 0x0a,
     177,  169,    8,    8, 0x2a,
     199,    8,    8,    8, 0x2a,
     210,    8,   31,    8, 0x08,
     229,  224,    8,    8, 0x08,
     256,    8,    8,    8, 0x08,
     276,    8,    8,    8, 0x08,
     302,  294,    8,    8, 0x08,
     337,  330,    8,    8, 0x08,
     366,    8,    8,    8, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_wdgScan[] = {
    "wdgScan\0\0scanFileNameChanged()\0bool\0"
    "newFileName\0updateScanFileName(QString)\0"
    "promptSaveChanges()\0reloadConfiguration()\0"
    "topLeft,topRight\0autosave(QModelIndex,QModelIndex)\0"
    "topLeft\0autosave(QModelIndex)\0autosave()\0"
    "saveChanges()\0data\0processNewData(QByteArray)\0"
    "processCommitData()\0updatePlaceEdit()\0"
    "enabled\0processToggleAutosave(bool)\0"
    "active\0processActivateScanner(bool)\0"
    "processActivateScanner()\0"
};

void wdgScan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        wdgScan *_t = static_cast<wdgScan *>(_o);
        switch (_id) {
        case 0: _t->scanFileNameChanged(); break;
        case 1: { bool _r = _t->updateScanFileName((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->promptSaveChanges();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->reloadConfiguration(); break;
        case 4: _t->autosave((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 5: _t->autosave((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->autosave(); break;
        case 7: { bool _r = _t->saveChanges();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->processNewData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 9: _t->processCommitData(); break;
        case 10: _t->updatePlaceEdit(); break;
        case 11: _t->processToggleAutosave((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->processActivateScanner((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->processActivateScanner(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData wdgScan::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wdgScan::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgScan,
      qt_meta_data_wdgScan, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wdgScan::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wdgScan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wdgScan::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wdgScan))
        return static_cast<void*>(const_cast< wdgScan*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgScan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void wdgScan::scanFileNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'duckracer.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "duckracer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'duckracer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_duckracer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      22,   10,   10,   10, 0x0a,
      41,   10,   10,   10, 0x0a,
      59,   10,   10,   10, 0x0a,
      84,   10,   76,   10, 0x0a,
     107,   10,   76,   10, 0x0a,
     125,   10,   10,   10, 0x08,
     144,   10,   10,   10, 0x08,
     164,  158,   10,   10, 0x08,
     185,   10,   10,   10, 0x08,
     208,   10,   10,   10, 0x08,
     230,   10,   10,   10, 0x08,
     254,   10,   10,   10, 0x08,
     277,   10,   10,   10, 0x08,
     309,   10,   10,   10, 0x28,
     334,   10,   10,   10, 0x08,
     361,   10,   10,   10, 0x28,
     381,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_duckracer[] = {
    "duckracer\0\0openScan()\0openManagePrizes()\0"
    "openPrintLabels()\0openPrintLists()\0"
    "QString\0getPrizeListFileName()\0"
    "getScanFileName()\0processConfigure()\0"
    "processQuit()\0index\0processCloseTab(int)\0"
    "processOpenPrizeFile()\0processOpenScanFile()\0"
    "processClosePrizeFile()\0processCloseScanFile()\0"
    "checkPrizeListFileName(QString)\0"
    "checkPrizeListFileName()\0"
    "checkScanFileName(QString)\0"
    "checkScanFileName()\0updateFileNameLabels()\0"
};

void duckracer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        duckracer *_t = static_cast<duckracer *>(_o);
        switch (_id) {
        case 0: _t->openScan(); break;
        case 1: _t->openManagePrizes(); break;
        case 2: _t->openPrintLabels(); break;
        case 3: _t->openPrintLists(); break;
        case 4: { QString _r = _t->getPrizeListFileName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getScanFileName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: _t->processConfigure(); break;
        case 7: _t->processQuit(); break;
        case 8: _t->processCloseTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->processOpenPrizeFile(); break;
        case 10: _t->processOpenScanFile(); break;
        case 11: _t->processClosePrizeFile(); break;
        case 12: _t->processCloseScanFile(); break;
        case 13: _t->checkPrizeListFileName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->checkPrizeListFileName(); break;
        case 15: _t->checkScanFileName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->checkScanFileName(); break;
        case 17: _t->updateFileNameLabels(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData duckracer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject duckracer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_duckracer,
      qt_meta_data_duckracer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &duckracer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *duckracer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *duckracer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_duckracer))
        return static_cast<void*>(const_cast< duckracer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int duckracer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

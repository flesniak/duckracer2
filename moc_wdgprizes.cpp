/****************************************************************************
** Meta object code from reading C++ file 'wdgprizes.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wdgprizes.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgprizes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wdgPrizes[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   43,   38,   10, 0x0a,
      88,   10,   38,   10, 0x0a,
     108,   10,   10,   10, 0x08,
     124,   10,   10,   10, 0x08,
     143,   10,   10,   10, 0x08,
     162,   10,   10,   10, 0x08,
     183,   10,   10,   10, 0x08,
     213,  205,   10,   10, 0x08,
     238,   10,   38,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wdgPrizes[] = {
    "wdgPrizes\0\0prizeListFileNameChanged()\0"
    "bool\0newFileName\0updatePrizeListFileName(QString)\0"
    "promptSaveChanges()\0processAddRow()\0"
    "processRemoveRow()\0processMoveRowUp()\0"
    "processMoveRowDown()\0processDuplicateRow()\0"
    "topLeft\0dataChanged(QModelIndex)\0"
    "saveChanges()\0"
};

void wdgPrizes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        wdgPrizes *_t = static_cast<wdgPrizes *>(_o);
        switch (_id) {
        case 0: _t->prizeListFileNameChanged(); break;
        case 1: { bool _r = _t->updatePrizeListFileName((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->promptSaveChanges();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->processAddRow(); break;
        case 4: _t->processRemoveRow(); break;
        case 5: _t->processMoveRowUp(); break;
        case 6: _t->processMoveRowDown(); break;
        case 7: _t->processDuplicateRow(); break;
        case 8: _t->dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: { bool _r = _t->saveChanges();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData wdgPrizes::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject wdgPrizes::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgPrizes,
      qt_meta_data_wdgPrizes, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wdgPrizes::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wdgPrizes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wdgPrizes::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wdgPrizes))
        return static_cast<void*>(const_cast< wdgPrizes*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgPrizes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void wdgPrizes::prizeListFileNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ViewerPBS/glwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GLWidget_t {
    QByteArrayData data[25];
    char stringdata0[285];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLWidget_t qt_meta_stringdata_GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GLWidget"
QT_MOC_LITERAL(1, 9, 8), // "SetFaces"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 11), // "SetVertices"
QT_MOC_LITERAL(4, 31, 12), // "SetFramerate"
QT_MOC_LITERAL(5, 44, 13), // "SetDrawOctree"
QT_MOC_LITERAL(6, 58, 18), // "SetDrawOctreeByLvl"
QT_MOC_LITERAL(7, 77, 7), // "SetMean"
QT_MOC_LITERAL(8, 85, 11), // "SetQuadrics"
QT_MOC_LITERAL(9, 97, 7), // "paintGL"
QT_MOC_LITERAL(10, 105, 13), // "SetReflection"
QT_MOC_LITERAL(11, 119, 3), // "set"
QT_MOC_LITERAL(12, 123, 7), // "SetBRDF"
QT_MOC_LITERAL(13, 131, 11), // "SetFresnelR"
QT_MOC_LITERAL(14, 143, 11), // "SetFresnelB"
QT_MOC_LITERAL(15, 155, 11), // "SetFresnelG"
QT_MOC_LITERAL(16, 167, 14), // "SetNumElements"
QT_MOC_LITERAL(17, 182, 12), // "SetRoughness"
QT_MOC_LITERAL(18, 195, 12), // "SetMetalness"
QT_MOC_LITERAL(19, 208, 17), // "CreateSSAOSamples"
QT_MOC_LITERAL(20, 226, 11), // "SetLODLevel"
QT_MOC_LITERAL(21, 238, 10), // "DrawOctree"
QT_MOC_LITERAL(22, 249, 15), // "DrawOctreeByLvl"
QT_MOC_LITERAL(23, 265, 7), // "UseMean"
QT_MOC_LITERAL(24, 273, 11) // "UseQuadrics"

    },
    "GLWidget\0SetFaces\0\0SetVertices\0"
    "SetFramerate\0SetDrawOctree\0"
    "SetDrawOctreeByLvl\0SetMean\0SetQuadrics\0"
    "paintGL\0SetReflection\0set\0SetBRDF\0"
    "SetFresnelR\0SetFresnelB\0SetFresnelG\0"
    "SetNumElements\0SetRoughness\0SetMetalness\0"
    "CreateSSAOSamples\0SetLODLevel\0DrawOctree\0"
    "DrawOctreeByLvl\0UseMean\0UseQuadrics"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06 /* Public */,
       3,    1,  127,    2, 0x06 /* Public */,
       4,    1,  130,    2, 0x06 /* Public */,
       5,    1,  133,    2, 0x06 /* Public */,
       6,    1,  136,    2, 0x06 /* Public */,
       7,    1,  139,    2, 0x06 /* Public */,
       8,    1,  142,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  145,    2, 0x09 /* Protected */,
      10,    1,  146,    2, 0x09 /* Protected */,
      12,    1,  149,    2, 0x09 /* Protected */,
      13,    1,  152,    2, 0x09 /* Protected */,
      14,    1,  155,    2, 0x09 /* Protected */,
      15,    1,  158,    2, 0x09 /* Protected */,
      16,    1,  161,    2, 0x09 /* Protected */,
      17,    1,  164,    2, 0x09 /* Protected */,
      18,    1,  167,    2, 0x09 /* Protected */,
      19,    0,  170,    2, 0x09 /* Protected */,
      20,    1,  171,    2, 0x09 /* Protected */,
      21,    1,  174,    2, 0x09 /* Protected */,
      22,    1,  177,    2, 0x09 /* Protected */,
      23,    1,  180,    2, 0x09 /* Protected */,
      24,    1,  183,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWidget *_t = static_cast<GLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetFaces((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->SetVertices((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->SetFramerate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->SetDrawOctree((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->SetDrawOctreeByLvl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->SetMean((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->SetQuadrics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->paintGL(); break;
        case 8: _t->SetReflection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->SetBRDF((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->SetFresnelR((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->SetFresnelB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->SetFresnelG((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->SetNumElements((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->SetRoughness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: _t->SetMetalness((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->CreateSSAOSamples(); break;
        case 17: _t->SetLODLevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->DrawOctree((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->DrawOctreeByLvl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->UseMean((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->UseQuadrics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetFaces)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetVertices)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetFramerate)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetDrawOctree)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetDrawOctreeByLvl)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetMean)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (GLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::SetQuadrics)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject GLWidget::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_GLWidget.data,
      qt_meta_data_GLWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget.stringdata0))
        return static_cast<void*>(this);
    return QGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::SetFaces(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GLWidget::SetVertices(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GLWidget::SetFramerate(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GLWidget::SetDrawOctree(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GLWidget::SetDrawOctreeByLvl(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GLWidget::SetMean(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GLWidget::SetQuadrics(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

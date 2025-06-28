/****************************************************************************
** Meta object code from reading C++ file 'chatroom.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../chatroom.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatroom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChatRoom_t {
    const uint offsetsAndSize[24];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ChatRoom_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ChatRoom_t qt_meta_stringdata_ChatRoom = {
    {
QT_MOC_LITERAL(0, 8), // "ChatRoom"
QT_MOC_LITERAL(9, 12), // "aboutToClose"
QT_MOC_LITERAL(22, 0), // ""
QT_MOC_LITERAL(23, 9), // "ChatRoom*"
QT_MOC_LITERAL(33, 4), // "room"
QT_MOC_LITERAL(38, 18), // "unreadCountChanged"
QT_MOC_LITERAL(57, 11), // "sendMessage"
QT_MOC_LITERAL(69, 14), // "displayMessage"
QT_MOC_LITERAL(84, 11), // "ChatMessage"
QT_MOC_LITERAL(96, 7), // "message"
QT_MOC_LITERAL(104, 11), // "saveHistory"
QT_MOC_LITERAL(116, 15) // "saveUnreadState"

    },
    "ChatRoom\0aboutToClose\0\0ChatRoom*\0room\0"
    "unreadCountChanged\0sendMessage\0"
    "displayMessage\0ChatMessage\0message\0"
    "saveHistory\0saveUnreadState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatRoom[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    0,   53,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   54,    2, 0x08,    4 /* Private */,
       7,    1,   55,    2, 0x08,    5 /* Private */,
      10,    0,   58,    2, 0x08,    7 /* Private */,
      11,    0,   59,    2, 0x08,    8 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChatRoom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatRoom *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->aboutToClose((*reinterpret_cast< ChatRoom*(*)>(_a[1]))); break;
        case 1: _t->unreadCountChanged(); break;
        case 2: _t->sendMessage(); break;
        case 3: _t->displayMessage((*reinterpret_cast< const ChatMessage(*)>(_a[1]))); break;
        case 4: _t->saveHistory(); break;
        case 5: _t->saveUnreadState(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< ChatRoom* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatRoom::*)(ChatRoom * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatRoom::aboutToClose)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChatRoom::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatRoom::unreadCountChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ChatRoom::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ChatRoom.offsetsAndSize,
    qt_meta_data_ChatRoom,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ChatRoom_t
, QtPrivate::TypeAndForceComplete<ChatRoom, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<ChatRoom *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const ChatMessage &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ChatRoom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatRoom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatRoom.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatRoom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ChatRoom::aboutToClose(ChatRoom * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChatRoom::unreadCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

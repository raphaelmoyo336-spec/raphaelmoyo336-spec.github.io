/****************************************************************************
** Meta object code from reading C++ file 'Session.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../external/qtermwidget/lib/Session.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Session.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7Konsole7SessionE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::Session::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole7SessionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::Session",
        "started",
        "",
        "finished",
        "receivedData",
        "text",
        "titleChanged",
        "currentDirectoryChanged",
        "dir",
        "profileChanged",
        "profile",
        "stateChanged",
        "state",
        "bellRequest",
        "message",
        "changeTabTextColorRequest",
        "changeBackgroundColorRequest",
        "QColor",
        "openUrlRequest",
        "url",
        "resizeRequest",
        "QSize",
        "size",
        "profileChangeCommandReceived",
        "flowControlEnabledChanged",
        "enabled",
        "cursorChanged",
        "Emulation::KeyboardCursorShape",
        "cursorShape",
        "blinkingCursorEnabled",
        "silence",
        "activity",
        "run",
        "runEmptyPTY",
        "close",
        "setUserTitle",
        "caption",
        "done",
        "QProcess::ExitStatus",
        "onReceiveBlock",
        "const char*",
        "buffer",
        "len",
        "monitorTimerDone",
        "onViewSizeChange",
        "height",
        "width",
        "onEmulationSizeChange",
        "postColor",
        "forWhat",
        "setColor",
        "color",
        "activityStateSet",
        "viewDestroyed",
        "view",
        "name",
        "processId",
        "keyBindings"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'started'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'finished'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'receivedData'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'titleChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentDirectoryChanged'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Signal 'profileChanged'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'stateChanged'
        QtMocHelpers::SignalData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Signal 'bellRequest'
        QtMocHelpers::SignalData<void(const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Signal 'changeTabTextColorRequest'
        QtMocHelpers::SignalData<void(int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Signal 'changeBackgroundColorRequest'
        QtMocHelpers::SignalData<void(const QColor &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 2 },
        }}),
        // Signal 'openUrlRequest'
        QtMocHelpers::SignalData<void(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Signal 'resizeRequest'
        QtMocHelpers::SignalData<void(const QSize &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Signal 'profileChangeCommandReceived'
        QtMocHelpers::SignalData<void(const QString &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'flowControlEnabledChanged'
        QtMocHelpers::SignalData<void(bool)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 25 },
        }}),
        // Signal 'cursorChanged'
        QtMocHelpers::SignalData<void(Emulation::KeyboardCursorShape, bool)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 27, 28 }, { QMetaType::Bool, 29 },
        }}),
        // Signal 'silence'
        QtMocHelpers::SignalData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'activity'
        QtMocHelpers::SignalData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'run'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'runEmptyPTY'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'close'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setUserTitle'
        QtMocHelpers::SlotData<void(int, const QString &)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { QMetaType::QString, 36 },
        }}),
        // Slot 'done'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 }, { 0x80000000 | 38, 2 },
        }}),
        // Slot 'onReceiveBlock'
        QtMocHelpers::SlotData<void(const char *, int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 40, 41 }, { QMetaType::Int, 42 },
        }}),
        // Slot 'monitorTimerDone'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onViewSizeChange'
        QtMocHelpers::SlotData<void(int, int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 45 }, { QMetaType::Int, 46 },
        }}),
        // Slot 'onEmulationSizeChange'
        QtMocHelpers::SlotData<void(QSize)>(47, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 21, 2 },
        }}),
        // Slot 'postColor'
        QtMocHelpers::SlotData<void(int)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 49 },
        }}),
        // Slot 'setColor'
        QtMocHelpers::SlotData<void(int, QString)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 49 }, { QMetaType::QString, 51 },
        }}),
        // Slot 'activityStateSet'
        QtMocHelpers::SlotData<void(int)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'viewDestroyed'
        QtMocHelpers::SlotData<void(QObject *)>(53, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QObjectStar, 54 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'name'
        QtMocHelpers::PropertyData<QString>(55, QMetaType::QString, QMC::DefaultPropertyFlags),
        // property 'processId'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags),
        // property 'keyBindings'
        QtMocHelpers::PropertyData<QString>(57, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet),
        // property 'size'
        QtMocHelpers::PropertyData<QSize>(22, 0x80000000 | 21, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Session, qt_meta_tag_ZN7Konsole7SessionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::Session::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole7SessionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole7SessionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole7SessionE_t>.metaTypes,
    nullptr
} };

void Konsole::Session::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Session *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->finished(); break;
        case 2: _t->receivedData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->titleChanged(); break;
        case 4: _t->currentDirectoryChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->profileChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->stateChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->bellRequest((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->changeTabTextColorRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->changeBackgroundColorRequest((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1]))); break;
        case 10: _t->openUrlRequest((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->resizeRequest((*reinterpret_cast<std::add_pointer_t<QSize>>(_a[1]))); break;
        case 12: _t->profileChangeCommandReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->flowControlEnabledChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->cursorChanged((*reinterpret_cast<std::add_pointer_t<Emulation::KeyboardCursorShape>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 15: _t->silence(); break;
        case 16: _t->activity(); break;
        case 17: _t->run(); break;
        case 18: _t->runEmptyPTY(); break;
        case 19: _t->close(); break;
        case 20: _t->setUserTitle((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->done((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 22: _t->onReceiveBlock((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 23: _t->monitorTimerDone(); break;
        case 24: _t->onViewSizeChange((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 25: _t->onEmulationSizeChange((*reinterpret_cast<std::add_pointer_t<QSize>>(_a[1]))); break;
        case 26: _t->postColor((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->setColor((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 28: _t->activityStateSet((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->viewDestroyed((*reinterpret_cast<std::add_pointer_t<QObject*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Session::*)()>(_a, &Session::started, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)()>(_a, &Session::finished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::receivedData, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)()>(_a, &Session::titleChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::currentDirectoryChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::profileChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(int )>(_a, &Session::stateChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::bellRequest, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(int )>(_a, &Session::changeTabTextColorRequest, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QColor & )>(_a, &Session::changeBackgroundColorRequest, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::openUrlRequest, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QSize & )>(_a, &Session::resizeRequest, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(const QString & )>(_a, &Session::profileChangeCommandReceived, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(bool )>(_a, &Session::flowControlEnabledChanged, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)(Emulation::KeyboardCursorShape , bool )>(_a, &Session::cursorChanged, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)()>(_a, &Session::silence, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Session::*)()>(_a, &Session::activity, 16))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->nameTitle(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->processId(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->keyBindings(); break;
        case 3: *reinterpret_cast<QSize*>(_v) = _t->size(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setKeyBindings(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setSize(*reinterpret_cast<QSize*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Konsole::Session::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::Session::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole7SessionE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Konsole::Session::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 30;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Konsole::Session::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Konsole::Session::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Konsole::Session::receivedData(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Konsole::Session::titleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Konsole::Session::currentDirectoryChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Konsole::Session::profileChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void Konsole::Session::stateChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Konsole::Session::bellRequest(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Konsole::Session::changeTabTextColorRequest(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void Konsole::Session::changeBackgroundColorRequest(const QColor & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void Konsole::Session::openUrlRequest(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void Konsole::Session::resizeRequest(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void Konsole::Session::profileChangeCommandReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void Konsole::Session::flowControlEnabledChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void Konsole::Session::cursorChanged(Emulation::KeyboardCursorShape _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1, _t2);
}

// SIGNAL 15
void Konsole::Session::silence()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}

// SIGNAL 16
void Konsole::Session::activity()
{
    QMetaObject::activate(this, &staticMetaObject, 16, nullptr);
}
namespace {
struct qt_meta_tag_ZN7Konsole12SessionGroupE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::SessionGroup::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole12SessionGroupE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::SessionGroup"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SessionGroup, qt_meta_tag_ZN7Konsole12SessionGroupE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::SessionGroup::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole12SessionGroupE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole12SessionGroupE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole12SessionGroupE_t>.metaTypes,
    nullptr
} };

void Konsole::SessionGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SessionGroup *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Konsole::SessionGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::SessionGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole12SessionGroupE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Konsole::SessionGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP

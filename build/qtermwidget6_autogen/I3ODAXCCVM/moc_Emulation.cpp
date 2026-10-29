/****************************************************************************
** Meta object code from reading C++ file 'Emulation.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../external/qtermwidget/lib/Emulation.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Emulation.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7Konsole9EmulationE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::Emulation::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole9EmulationE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::Emulation",
        "sendData",
        "",
        "const char*",
        "data",
        "len",
        "lockPtyRequest",
        "suspend",
        "useUtf8Request",
        "stateSet",
        "state",
        "zmodemDetected",
        "changeTabTextColorRequest",
        "color",
        "programUsesMouseChanged",
        "usesMouse",
        "programBracketedPasteModeChanged",
        "bracketedPasteMode",
        "outputChanged",
        "titleChanged",
        "title",
        "newTitle",
        "imageSizeChanged",
        "lineCount",
        "columnCount",
        "imageSizeInitialized",
        "imageResizeRequest",
        "QSize",
        "sizz",
        "profileChangeCommandReceived",
        "text",
        "flowControlKeyPressed",
        "suspendKeyPressed",
        "cursorChanged",
        "KeyboardCursorShape",
        "cursorShape",
        "blinkingCursorEnabled",
        "oscColorQuery",
        "forWhat",
        "oscColorChangeRequest",
        "handleCommandFromKeyboard",
        "KeyboardTranslator::Command",
        "command",
        "outputFromKeypressEvent",
        "setImageSize",
        "lines",
        "columns",
        "sendText",
        "sendKeyEvent",
        "QKeyEvent*",
        "fromPaste",
        "sendMouseEvent",
        "buttons",
        "column",
        "line",
        "eventType",
        "sendString",
        "string",
        "length",
        "receiveData",
        "buffer",
        "bufferedUpdate",
        "showBulk",
        "usesMouseChanged",
        "bracketedPasteModeChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sendData'
        QtMocHelpers::SignalData<void(const char *, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'lockPtyRequest'
        QtMocHelpers::SignalData<void(bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 },
        }}),
        // Signal 'useUtf8Request'
        QtMocHelpers::SignalData<void(bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Signal 'stateSet'
        QtMocHelpers::SignalData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Signal 'zmodemDetected'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'changeTabTextColorRequest'
        QtMocHelpers::SignalData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Signal 'programUsesMouseChanged'
        QtMocHelpers::SignalData<void(bool)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Signal 'programBracketedPasteModeChanged'
        QtMocHelpers::SignalData<void(bool)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 17 },
        }}),
        // Signal 'outputChanged'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'titleChanged'
        QtMocHelpers::SignalData<void(int, const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::QString, 21 },
        }}),
        // Signal 'imageSizeChanged'
        QtMocHelpers::SignalData<void(int, int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 },
        }}),
        // Signal 'imageSizeInitialized'
        QtMocHelpers::SignalData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'imageResizeRequest'
        QtMocHelpers::SignalData<void(const QSize &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 27, 28 },
        }}),
        // Signal 'profileChangeCommandReceived'
        QtMocHelpers::SignalData<void(const QString &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 },
        }}),
        // Signal 'flowControlKeyPressed'
        QtMocHelpers::SignalData<void(bool)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 32 },
        }}),
        // Signal 'cursorChanged'
        QtMocHelpers::SignalData<void(enum KeyboardCursorShape, bool)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 34, 35 }, { QMetaType::Bool, 36 },
        }}),
        // Signal 'oscColorQuery'
        QtMocHelpers::SignalData<void(int)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 38 },
        }}),
        // Signal 'oscColorChangeRequest'
        QtMocHelpers::SignalData<void(int, QString)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 38 }, { QMetaType::QString, 13 },
        }}),
        // Signal 'handleCommandFromKeyboard'
        QtMocHelpers::SignalData<void(KeyboardTranslator::Command)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 41, 42 },
        }}),
        // Signal 'outputFromKeypressEvent'
        QtMocHelpers::SignalData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setImageSize'
        QtMocHelpers::SlotData<void(int, int)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 45 }, { QMetaType::Int, 46 },
        }}),
        // Slot 'sendText'
        QtMocHelpers::SlotData<void(const QString &)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 },
        }}),
        // Slot 'sendKeyEvent'
        QtMocHelpers::SlotData<void(QKeyEvent *, bool)>(48, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 49, 2 }, { QMetaType::Bool, 50 },
        }}),
        // Slot 'sendMouseEvent'
        QtMocHelpers::SlotData<void(int, int, int, int)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 52 }, { QMetaType::Int, 53 }, { QMetaType::Int, 54 }, { QMetaType::Int, 55 },
        }}),
        // Slot 'sendString'
        QtMocHelpers::SlotData<void(const char *, int)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 57 }, { QMetaType::Int, 58 },
        }}),
        // Slot 'sendString'
        QtMocHelpers::SlotData<void(const char *)>(56, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 3, 57 },
        }}),
        // Slot 'receiveData'
        QtMocHelpers::SlotData<void(const char *, int)>(59, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 60 }, { QMetaType::Int, 5 },
        }}),
        // Slot 'bufferedUpdate'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'showBulk'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'usesMouseChanged'
        QtMocHelpers::SlotData<void(bool)>(63, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 15 },
        }}),
        // Slot 'bracketedPasteModeChanged'
        QtMocHelpers::SlotData<void(bool)>(64, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 17 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Emulation, qt_meta_tag_ZN7Konsole9EmulationE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::Emulation::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9EmulationE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9EmulationE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole9EmulationE_t>.metaTypes,
    nullptr
} };

void Konsole::Emulation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Emulation *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sendData((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->lockPtyRequest((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->useUtf8Request((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->stateSet((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->zmodemDetected(); break;
        case 5: _t->changeTabTextColorRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->programUsesMouseChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->programBracketedPasteModeChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->outputChanged(); break;
        case 9: _t->titleChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->imageSizeChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 11: _t->imageSizeInitialized(); break;
        case 12: _t->imageResizeRequest((*reinterpret_cast<std::add_pointer_t<QSize>>(_a[1]))); break;
        case 13: _t->profileChangeCommandReceived((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->flowControlKeyPressed((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 15: _t->cursorChanged((*reinterpret_cast<std::add_pointer_t<enum KeyboardCursorShape>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 16: _t->oscColorQuery((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->oscColorChangeRequest((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 18: _t->handleCommandFromKeyboard((*reinterpret_cast<std::add_pointer_t<KeyboardTranslator::Command>>(_a[1]))); break;
        case 19: _t->outputFromKeypressEvent(); break;
        case 20: _t->setImageSize((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 21: _t->sendText((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->sendKeyEvent((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 23: _t->sendMouseEvent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 24: _t->sendString((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 25: _t->sendString((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1]))); break;
        case 26: _t->receiveData((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 27: _t->bufferedUpdate(); break;
        case 28: _t->showBulk(); break;
        case 29: _t->usesMouseChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 30: _t->bracketedPasteModeChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(const char * , int )>(_a, &Emulation::sendData, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(bool )>(_a, &Emulation::lockPtyRequest, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(bool )>(_a, &Emulation::useUtf8Request, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int )>(_a, &Emulation::stateSet, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)()>(_a, &Emulation::zmodemDetected, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int )>(_a, &Emulation::changeTabTextColorRequest, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(bool )>(_a, &Emulation::programUsesMouseChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(bool )>(_a, &Emulation::programBracketedPasteModeChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)()>(_a, &Emulation::outputChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int , const QString & )>(_a, &Emulation::titleChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int , int )>(_a, &Emulation::imageSizeChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)()>(_a, &Emulation::imageSizeInitialized, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(const QSize & )>(_a, &Emulation::imageResizeRequest, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(const QString & )>(_a, &Emulation::profileChangeCommandReceived, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(bool )>(_a, &Emulation::flowControlKeyPressed, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(KeyboardCursorShape , bool )>(_a, &Emulation::cursorChanged, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int )>(_a, &Emulation::oscColorQuery, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(int , QString )>(_a, &Emulation::oscColorChangeRequest, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)(KeyboardTranslator::Command )>(_a, &Emulation::handleCommandFromKeyboard, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (Emulation::*)()>(_a, &Emulation::outputFromKeypressEvent, 19))
            return;
    }
}

const QMetaObject *Konsole::Emulation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::Emulation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9EmulationE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Konsole::Emulation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void Konsole::Emulation::sendData(const char * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Konsole::Emulation::lockPtyRequest(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Konsole::Emulation::useUtf8Request(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void Konsole::Emulation::stateSet(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void Konsole::Emulation::zmodemDetected()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Konsole::Emulation::changeTabTextColorRequest(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void Konsole::Emulation::programUsesMouseChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Konsole::Emulation::programBracketedPasteModeChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Konsole::Emulation::outputChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Konsole::Emulation::titleChanged(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void Konsole::Emulation::imageSizeChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1, _t2);
}

// SIGNAL 11
void Konsole::Emulation::imageSizeInitialized()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void Konsole::Emulation::imageResizeRequest(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void Konsole::Emulation::profileChangeCommandReceived(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void Konsole::Emulation::flowControlKeyPressed(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1);
}

// SIGNAL 15
void Konsole::Emulation::cursorChanged(KeyboardCursorShape _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1, _t2);
}

// SIGNAL 16
void Konsole::Emulation::oscColorQuery(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1);
}

// SIGNAL 17
void Konsole::Emulation::oscColorChangeRequest(int _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 17, nullptr, _t1, _t2);
}

// SIGNAL 18
void Konsole::Emulation::handleCommandFromKeyboard(KeyboardTranslator::Command _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1);
}

// SIGNAL 19
void Konsole::Emulation::outputFromKeypressEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}
QT_WARNING_POP

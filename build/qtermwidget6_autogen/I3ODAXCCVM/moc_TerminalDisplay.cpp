/****************************************************************************
** Meta object code from reading C++ file 'TerminalDisplay.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../external/qtermwidget/lib/TerminalDisplay.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerminalDisplay.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7Konsole15TerminalDisplayE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::TerminalDisplay::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::TerminalDisplay",
        "keyPressedSignal",
        "",
        "QKeyEvent*",
        "e",
        "fromPaste",
        "mouseSignal",
        "button",
        "column",
        "line",
        "eventType",
        "changedFontMetricSignal",
        "height",
        "width",
        "changedContentSizeSignal",
        "configureRequest",
        "QPoint",
        "position",
        "overrideShortcutCheck",
        "keyEvent",
        "bool&",
        "override",
        "isBusySelecting",
        "sendStringToEmu",
        "const char*",
        "copyAvailable",
        "termGetFocus",
        "termLostFocus",
        "notifyBell",
        "usesMouseChanged",
        "updateImage",
        "updateFilters",
        "updateLineProperties",
        "copyClipboard",
        "pasteClipboard",
        "pasteSelection",
        "setFlowControlWarningEnabled",
        "enabled",
        "flowControlWarningEnabled",
        "outputSuspended",
        "suspended",
        "setUsesMouse",
        "usesMouse",
        "setBracketedPasteMode",
        "bracketedPasteMode",
        "bell",
        "message",
        "setBackgroundColor",
        "QColor",
        "color",
        "backgroundColor",
        "setForegroundColor",
        "foregroundColor",
        "selectionChanged",
        "scrollBarPositionChanged",
        "value",
        "blinkEvent",
        "blinkCursorEvent",
        "enableBell",
        "swapColorTable",
        "tripleClickTimeout",
        "showPendingLinkTooltip"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'keyPressedSignal'
        QtMocHelpers::SignalData<void(QKeyEvent *, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::Bool, 5 },
        }}),
        // Signal 'mouseSignal'
        QtMocHelpers::SignalData<void(int, int, int, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 }, { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Signal 'changedFontMetricSignal'
        QtMocHelpers::SignalData<void(int, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'changedContentSizeSignal'
        QtMocHelpers::SignalData<void(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Signal 'configureRequest'
        QtMocHelpers::SignalData<void(const QPoint &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Signal 'overrideShortcutCheck'
        QtMocHelpers::SignalData<void(QKeyEvent *, bool &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 19 }, { 0x80000000 | 20, 21 },
        }}),
        // Signal 'isBusySelecting'
        QtMocHelpers::SignalData<void(bool)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Signal 'sendStringToEmu'
        QtMocHelpers::SignalData<void(const char *)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 2 },
        }}),
        // Signal 'copyAvailable'
        QtMocHelpers::SignalData<void(bool)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Signal 'termGetFocus'
        QtMocHelpers::SignalData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'termLostFocus'
        QtMocHelpers::SignalData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'notifyBell'
        QtMocHelpers::SignalData<void(const QString &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Signal 'usesMouseChanged'
        QtMocHelpers::SignalData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateImage'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateFilters'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateLineProperties'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'copyClipboard'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pasteClipboard'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pasteSelection'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setFlowControlWarningEnabled'
        QtMocHelpers::SlotData<void(bool)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 37 },
        }}),
        // Slot 'flowControlWarningEnabled'
        QtMocHelpers::SlotData<bool() const>(38, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'outputSuspended'
        QtMocHelpers::SlotData<void(bool)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 40 },
        }}),
        // Slot 'setUsesMouse'
        QtMocHelpers::SlotData<void(bool)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 42 },
        }}),
        // Slot 'usesMouse'
        QtMocHelpers::SlotData<bool() const>(42, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'setBracketedPasteMode'
        QtMocHelpers::SlotData<void(bool)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 44 },
        }}),
        // Slot 'bracketedPasteMode'
        QtMocHelpers::SlotData<bool() const>(44, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'bell'
        QtMocHelpers::SlotData<void(const QString &)>(45, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 46 },
        }}),
        // Slot 'setBackgroundColor'
        QtMocHelpers::SlotData<void(const QColor &)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 48, 49 },
        }}),
        // Slot 'backgroundColor'
        QtMocHelpers::SlotData<QColor() const>(50, 2, QMC::AccessPublic, 0x80000000 | 48),
        // Slot 'setForegroundColor'
        QtMocHelpers::SlotData<void(const QColor &)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 48, 49 },
        }}),
        // Slot 'foregroundColor'
        QtMocHelpers::SlotData<QColor() const>(52, 2, QMC::AccessPublic, 0x80000000 | 48),
        // Slot 'selectionChanged'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'scrollBarPositionChanged'
        QtMocHelpers::SlotData<void(int)>(54, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 55 },
        }}),
        // Slot 'blinkEvent'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'blinkCursorEvent'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'enableBell'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'swapColorTable'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'tripleClickTimeout'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showPendingLinkTooltip'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TerminalDisplay, qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::TerminalDisplay::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>.metaTypes,
    nullptr
} };

void Konsole::TerminalDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TerminalDisplay *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->keyPressedSignal((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 1: _t->mouseSignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 2: _t->changedFontMetricSignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->changedContentSizeSignal((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 4: _t->configureRequest((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 5: _t->overrideShortcutCheck((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool&>>(_a[2]))); break;
        case 6: _t->isBusySelecting((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->sendStringToEmu((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1]))); break;
        case 8: _t->copyAvailable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->termGetFocus(); break;
        case 10: _t->termLostFocus(); break;
        case 11: _t->notifyBell((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->usesMouseChanged(); break;
        case 13: _t->updateImage(); break;
        case 14: _t->updateFilters(); break;
        case 15: _t->updateLineProperties(); break;
        case 16: _t->copyClipboard(); break;
        case 17: _t->pasteClipboard(); break;
        case 18: _t->pasteSelection(); break;
        case 19: _t->setFlowControlWarningEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 20: { bool _r = _t->flowControlWarningEnabled();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->outputSuspended((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->setUsesMouse((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: { bool _r = _t->usesMouse();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->setBracketedPasteMode((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 25: { bool _r = _t->bracketedPasteMode();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 26: _t->bell((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->setBackgroundColor((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1]))); break;
        case 28: { QColor _r = _t->backgroundColor();
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 29: _t->setForegroundColor((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1]))); break;
        case 30: { QColor _r = _t->foregroundColor();
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 31: _t->selectionChanged(); break;
        case 32: _t->scrollBarPositionChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 33: _t->blinkEvent(); break;
        case 34: _t->blinkCursorEvent(); break;
        case 35: _t->enableBell(); break;
        case 36: _t->swapColorTable(); break;
        case 37: _t->tripleClickTimeout(); break;
        case 38: _t->showPendingLinkTooltip(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(QKeyEvent * , bool )>(_a, &TerminalDisplay::keyPressedSignal, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(int , int , int , int )>(_a, &TerminalDisplay::mouseSignal, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(int , int )>(_a, &TerminalDisplay::changedFontMetricSignal, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(int , int )>(_a, &TerminalDisplay::changedContentSizeSignal, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(const QPoint & )>(_a, &TerminalDisplay::configureRequest, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(QKeyEvent * , bool & )>(_a, &TerminalDisplay::overrideShortcutCheck, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(bool )>(_a, &TerminalDisplay::isBusySelecting, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(const char * )>(_a, &TerminalDisplay::sendStringToEmu, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(bool )>(_a, &TerminalDisplay::copyAvailable, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)()>(_a, &TerminalDisplay::termGetFocus, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)()>(_a, &TerminalDisplay::termLostFocus, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)(const QString & )>(_a, &TerminalDisplay::notifyBell, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (TerminalDisplay::*)()>(_a, &TerminalDisplay::usesMouseChanged, 12))
            return;
    }
}

const QMetaObject *Konsole::TerminalDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::TerminalDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole15TerminalDisplayE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Konsole::TerminalDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void Konsole::TerminalDisplay::keyPressedSignal(QKeyEvent * _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void Konsole::TerminalDisplay::mouseSignal(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 2
void Konsole::TerminalDisplay::changedFontMetricSignal(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void Konsole::TerminalDisplay::changedContentSizeSignal(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void Konsole::TerminalDisplay::configureRequest(const QPoint & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void Konsole::TerminalDisplay::overrideShortcutCheck(QKeyEvent * _t1, bool & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void Konsole::TerminalDisplay::isBusySelecting(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Konsole::TerminalDisplay::sendStringToEmu(const char * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Konsole::TerminalDisplay::copyAvailable(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void Konsole::TerminalDisplay::termGetFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void Konsole::TerminalDisplay::termLostFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void Konsole::TerminalDisplay::notifyBell(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void Konsole::TerminalDisplay::usesMouseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
namespace {
struct qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::AutoScrollHandler::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::AutoScrollHandler"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AutoScrollHandler, qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::AutoScrollHandler::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>.metaTypes,
    nullptr
} };

void Konsole::AutoScrollHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AutoScrollHandler *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Konsole::AutoScrollHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::AutoScrollHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole17AutoScrollHandlerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Konsole::AutoScrollHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN7Konsole9ScrollBarE_t {};
} // unnamed namespace

template <> constexpr inline auto Konsole::ScrollBar::qt_create_metaobjectdata<qt_meta_tag_ZN7Konsole9ScrollBarE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Konsole::ScrollBar"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScrollBar, qt_meta_tag_ZN7Konsole9ScrollBarE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Konsole::ScrollBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QScrollBar::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9ScrollBarE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9ScrollBarE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7Konsole9ScrollBarE_t>.metaTypes,
    nullptr
} };

void Konsole::ScrollBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScrollBar *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *Konsole::ScrollBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Konsole::ScrollBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7Konsole9ScrollBarE_t>.strings))
        return static_cast<void*>(this);
    return QScrollBar::qt_metacast(_clname);
}

int Konsole::ScrollBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollBar::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP

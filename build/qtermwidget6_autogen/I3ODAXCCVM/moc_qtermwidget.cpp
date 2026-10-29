/****************************************************************************
** Meta object code from reading C++ file 'qtermwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../external/qtermwidget/lib/qtermwidget.h"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtermwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11QTermWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto QTermWidget::qt_create_metaobjectdata<qt_meta_tag_ZN11QTermWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QTermWidget",
        "finished",
        "",
        "copyAvailable",
        "termGetFocus",
        "termLostFocus",
        "termKeyPressed",
        "QKeyEvent*",
        "urlActivated",
        "QUrl",
        "fromContextMenu",
        "bell",
        "message",
        "activity",
        "silence",
        "sendData",
        "const char*",
        "profileChanged",
        "profile",
        "titleChanged",
        "currentDirectoryChanged",
        "dir",
        "receivedData",
        "text",
        "copyClipboard",
        "pasteClipboard",
        "pasteSelection",
        "zoomIn",
        "zoomOut",
        "setSize",
        "QSize",
        "setKeyBindings",
        "kb",
        "clear",
        "toggleShowSearchBar",
        "saveHistory",
        "QIODevice*",
        "device",
        "sessionFinished",
        "selectionChanged",
        "textSelected",
        "find",
        "findNext",
        "findPrevious",
        "matchFound",
        "startColumn",
        "startLine",
        "endColumn",
        "endLine",
        "noMatchFound",
        "cursorChanged",
        "Konsole::Emulation::KeyboardCursorShape",
        "cursorShape",
        "blinkingCursorEnabled"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'finished'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'copyAvailable'
        QtMocHelpers::SignalData<void(bool)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 2 },
        }}),
        // Signal 'termGetFocus'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'termLostFocus'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'termKeyPressed'
        QtMocHelpers::SignalData<void(QKeyEvent *)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 2 },
        }}),
        // Signal 'urlActivated'
        QtMocHelpers::SignalData<void(const QUrl &, bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 2 }, { QMetaType::Bool, 10 },
        }}),
        // Signal 'bell'
        QtMocHelpers::SignalData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Signal 'activity'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'silence'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sendData'
        QtMocHelpers::SignalData<void(const char *, int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 2 }, { QMetaType::Int, 2 },
        }}),
        // Signal 'profileChanged'
        QtMocHelpers::SignalData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Signal 'titleChanged'
        QtMocHelpers::SignalData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentDirectoryChanged'
        QtMocHelpers::SignalData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Signal 'receivedData'
        QtMocHelpers::SignalData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Slot 'copyClipboard'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pasteClipboard'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pasteSelection'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'zoomIn'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'zoomOut'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setSize'
        QtMocHelpers::SlotData<void(const QSize &)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 30, 2 },
        }}),
        // Slot 'setKeyBindings'
        QtMocHelpers::SlotData<void(const QString &)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 32 },
        }}),
        // Slot 'clear'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggleShowSearchBar'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'saveHistory'
        QtMocHelpers::SlotData<void(QIODevice *)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 36, 37 },
        }}),
        // Slot 'sessionFinished'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'selectionChanged'
        QtMocHelpers::SlotData<void(bool)>(39, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Bool, 40 },
        }}),
        // Slot 'find'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'findNext'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'findPrevious'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'matchFound'
        QtMocHelpers::SlotData<void(int, int, int, int)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 45 }, { QMetaType::Int, 46 }, { QMetaType::Int, 47 }, { QMetaType::Int, 48 },
        }}),
        // Slot 'noMatchFound'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'cursorChanged'
        QtMocHelpers::SlotData<void(Konsole::Emulation::KeyboardCursorShape, bool)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 51, 52 }, { QMetaType::Bool, 53 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QTermWidget, qt_meta_tag_ZN11QTermWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QTermWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QTermWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QTermWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11QTermWidgetE_t>.metaTypes,
    nullptr
} };

void QTermWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QTermWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->copyAvailable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->termGetFocus(); break;
        case 3: _t->termLostFocus(); break;
        case 4: _t->termKeyPressed((*reinterpret_cast<std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 5: _t->urlActivated((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->bell((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->activity(); break;
        case 8: _t->silence(); break;
        case 9: _t->sendData((*reinterpret_cast<std::add_pointer_t<const char*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->profileChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->titleChanged(); break;
        case 12: _t->currentDirectoryChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->receivedData((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->copyClipboard(); break;
        case 15: _t->pasteClipboard(); break;
        case 16: _t->pasteSelection(); break;
        case 17: _t->zoomIn(); break;
        case 18: _t->zoomOut(); break;
        case 19: _t->setSize((*reinterpret_cast<std::add_pointer_t<QSize>>(_a[1]))); break;
        case 20: _t->setKeyBindings((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->clear(); break;
        case 22: _t->toggleShowSearchBar(); break;
        case 23: _t->saveHistory((*reinterpret_cast<std::add_pointer_t<QIODevice*>>(_a[1]))); break;
        case 24: _t->sessionFinished(); break;
        case 25: _t->selectionChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 26: _t->find(); break;
        case 27: _t->findNext(); break;
        case 28: _t->findPrevious(); break;
        case 29: _t->matchFound((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 30: _t->noMatchFound(); break;
        case 31: _t->cursorChanged((*reinterpret_cast<std::add_pointer_t<Konsole::Emulation::KeyboardCursorShape>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 23:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QIODevice* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::finished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(bool )>(_a, &QTermWidget::copyAvailable, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::termGetFocus, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::termLostFocus, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(QKeyEvent * )>(_a, &QTermWidget::termKeyPressed, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const QUrl & , bool )>(_a, &QTermWidget::urlActivated, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const QString & )>(_a, &QTermWidget::bell, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::activity, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::silence, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const char * , int )>(_a, &QTermWidget::sendData, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const QString & )>(_a, &QTermWidget::profileChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)()>(_a, &QTermWidget::titleChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const QString & )>(_a, &QTermWidget::currentDirectoryChanged, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (QTermWidget::*)(const QString & )>(_a, &QTermWidget::receivedData, 13))
            return;
    }
}

const QMetaObject *QTermWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QTermWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11QTermWidgetE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QTermWidgetInterface"))
        return static_cast< QTermWidgetInterface*>(this);
    if (!strcmp(_clname, "lxqt.qtermwidget.QTermWidgetInterface/1.6"))
        return static_cast< QTermWidgetInterface*>(this);
    return QWidget::qt_metacast(_clname);
}

int QTermWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void QTermWidget::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QTermWidget::copyAvailable(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void QTermWidget::termGetFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QTermWidget::termLostFocus()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QTermWidget::termKeyPressed(QKeyEvent * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void QTermWidget::urlActivated(const QUrl & _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void QTermWidget::bell(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void QTermWidget::activity()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void QTermWidget::silence()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void QTermWidget::sendData(const char * _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void QTermWidget::profileChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void QTermWidget::titleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void QTermWidget::currentDirectoryChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void QTermWidget::receivedData(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_QTermWidget[] = {
    0xbf, 
    // "IID"
    0x02,  0x70,  'l',  'x',  'q',  't',  '.',  'q', 
    't',  'e',  'r',  'm',  'w',  'i',  'd',  'g', 
    'e',  't', 
    // "className"
    0x03,  0x6b,  'Q',  'T',  'e',  'r',  'm',  'W', 
    'i',  'd',  'g',  'e',  't', 
    // "MetaData"
    0x04,  0xa6,  0x69,  'C',  'o',  'p',  'y',  'r', 
    'i',  'g',  'h',  't',  0x72,  '(',  'C',  ')', 
    ' ',  '2',  '0',  '2',  '2',  '-',  '2',  '0', 
    '2',  '6',  ' ',  'L',  'X',  'Q',  't',  0x67, 
    'L',  'i',  'c',  'e',  'n',  's',  'e',  0x65, 
    'G',  'P',  'L',  'v',  '2',  0x64,  'N',  'a', 
    'm',  'e',  0x6b,  'Q',  'T',  'e',  'r',  'm', 
    'W',  'i',  'd',  'g',  'e',  't',  0x63,  'U', 
    'r',  'l',  0x78,  0x23,  'h',  't',  't',  'p', 
    's',  ':',  '/',  '/',  'g',  'i',  't',  'h', 
    'u',  'b',  '.',  'c',  'o',  'm',  '/',  'l', 
    'x',  'q',  't',  '/',  'q',  't',  'e',  'r', 
    'm',  'w',  'i',  'd',  'g',  'e',  't',  0x66, 
    'V',  'e',  'n',  'd',  'o',  'r',  0x64,  'L', 
    'X',  'Q',  't',  0x67,  'V',  'e',  'r',  's', 
    'i',  'o',  'n',  0x65,  '2',  '.',  '4',  '.', 
    '0', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(QTermWidget, QTermWidget, qt_pluginMetaDataV2_QTermWidget)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_QTermWidget[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x70,  'l',  'x',  'q',  't',  '.',  'q', 
    't',  'e',  'r',  'm',  'w',  'i',  'd',  'g', 
    'e',  't', 
    // "className"
    0x03,  0x6b,  'Q',  'T',  'e',  'r',  'm',  'W', 
    'i',  'd',  'g',  'e',  't', 
    // "MetaData"
    0x04,  0xa6,  0x69,  'C',  'o',  'p',  'y',  'r', 
    'i',  'g',  'h',  't',  0x72,  '(',  'C',  ')', 
    ' ',  '2',  '0',  '2',  '2',  '-',  '2',  '0', 
    '2',  '6',  ' ',  'L',  'X',  'Q',  't',  0x67, 
    'L',  'i',  'c',  'e',  'n',  's',  'e',  0x65, 
    'G',  'P',  'L',  'v',  '2',  0x64,  'N',  'a', 
    'm',  'e',  0x6b,  'Q',  'T',  'e',  'r',  'm', 
    'W',  'i',  'd',  'g',  'e',  't',  0x63,  'U', 
    'r',  'l',  0x78,  0x23,  'h',  't',  't',  'p', 
    's',  ':',  '/',  '/',  'g',  'i',  't',  'h', 
    'u',  'b',  '.',  'c',  'o',  'm',  '/',  'l', 
    'x',  'q',  't',  '/',  'q',  't',  'e',  'r', 
    'm',  'w',  'i',  'd',  'g',  'e',  't',  0x66, 
    'V',  'e',  'n',  'd',  'o',  'r',  0x64,  'L', 
    'X',  'Q',  't',  0x67,  'V',  'e',  'r',  's', 
    'i',  'o',  'n',  0x65,  '2',  '.',  '4',  '.', 
    '0', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(QTermWidget, QTermWidget)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP

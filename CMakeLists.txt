cmake_minimum_required(VERSION 3.16)
project(KogenceIDE LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.6 REQUIRED COMPONENTS Widgets WebEngineWidgets WebChannel PrintSupport)
set(QT_VERSION_MAJOR 6)

option(BUILD_BENCHMARKS "Build opt-in Kogence IDE benchmarks" OFF)

add_subdirectory(external/qtadvanceddocking)

find_package(QScintilla QUIET CONFIG)
if(TARGET QScintilla::QScintilla)
    set(KOGENCE_QSCINTILLA_TARGET QScintilla::QScintilla)
elseif(TARGET QScintilla)
    set(KOGENCE_QSCINTILLA_TARGET QScintilla)
else()
    find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h
        PATH_SUFFIXES QScintilla
    )
    find_library(QSCINTILLA_LIBRARY
        NAMES qscintilla2_qt6 qscintilla2-qt6
    )
    if(QSCINTILLA_INCLUDE_DIR AND QSCINTILLA_LIBRARY)
        add_library(QScintilla::QScintilla UNKNOWN IMPORTED)
        set_target_properties(QScintilla::QScintilla PROPERTIES
            IMPORTED_LOCATION "${QSCINTILLA_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${QSCINTILLA_INCLUDE_DIR}"
        )
        set(KOGENCE_QSCINTILLA_TARGET QScintilla::QScintilla)
    endif()
endif()

if(NOT KOGENCE_QSCINTILLA_TARGET
   AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/QScintilla/src/qscintilla.pro")
    set(QSCINTILLA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/QScintilla")
    file(GLOB QSCINTILLA_QT_SOURCES CONFIGURE_DEPENDS
        "${QSCINTILLA_SOURCE_DIR}/src/*.cpp"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/lexers/*.cpp"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/lexlib/*.cpp"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/src/*.cpp"
    )
    file(GLOB QSCINTILLA_HEADERS CONFIGURE_DEPENDS
        "${QSCINTILLA_SOURCE_DIR}/src/Qsci/*.h"
    )
    add_library(qscintilla STATIC ${QSCINTILLA_QT_SOURCES} ${QSCINTILLA_HEADERS})
    target_include_directories(qscintilla PUBLIC
        "${QSCINTILLA_SOURCE_DIR}/src"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/include"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/lexlib"
        "${QSCINTILLA_SOURCE_DIR}/scintilla/src"
    )
    target_compile_definitions(qscintilla PRIVATE
        SCINTILLA_QT
        SCI_LEXER
        INCLUDE_DEPRECATED_FEATURES
    )
    target_link_libraries(qscintilla PUBLIC Qt6::Widgets Qt6::PrintSupport)
    set(KOGENCE_QSCINTILLA_TARGET qscintilla)
elseif(NOT KOGENCE_QSCINTILLA_TARGET)
    message(FATAL_ERROR
        "QScintilla was not found. Install a Qt 6 QScintilla CMake package or "
        "provide its source under external/QScintilla.")
endif()

include(GenerateExportHeader)

set(QTERMWIDGET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/qtermwidget")
set(QTERMWIDGET_VERSION_MAJOR 2)
set(QTERMWIDGET_VERSION_MINOR 4)
set(QTERMWIDGET_VERSION_PATCH 0)
configure_file(
    "${QTERMWIDGET_SOURCE_DIR}/lib/qtermwidget_version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/qtermwidget/lib/qtermwidget_version.h"
    @ONLY
)

add_library(qtermwidget6 STATIC
    ${QTERMWIDGET_SOURCE_DIR}/lib/BlockArray.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/ColorScheme.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Emulation.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Filter.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/History.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/HistorySearch.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Hyperlink.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/KeyboardTranslator.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/konsole_wcwidth.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/kprocess.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/kpty.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/kptydevice.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/kptyprocess.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Pty.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/qtermwidget.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Screen.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/ScreenWindow.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/SearchBar.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Session.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/ShellCommand.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/TerminalCharacterDecoder.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/TerminalDisplay.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/tools.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Vt102Emulation.cpp
    ${QTERMWIDGET_SOURCE_DIR}/lib/Emulation.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/Filter.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/HistorySearch.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/kprocess.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/kptydevice.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/kptyprocess.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/Pty.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/qtermwidget.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/ScreenWindow.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/SearchBar.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/Session.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/TerminalDisplay.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/Vt102Emulation.h
    ${QTERMWIDGET_SOURCE_DIR}/lib/SearchBar.ui
    ${QTERMWIDGET_SOURCE_DIR}/lib/qtermwidget.json
)
generate_export_header(qtermwidget6
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/qtermwidget/lib/qtermwidget_export.h"
    BASE_NAME QTERMWIDGET
)
target_include_directories(qtermwidget6
    PUBLIC
        "${QTERMWIDGET_SOURCE_DIR}/lib"
        "${CMAKE_CURRENT_BINARY_DIR}/qtermwidget/lib"
)
target_compile_definitions(qtermwidget6
    PUBLIC
        QTERMWIDGET_STATIC_DEFINE
        "KOGENCE_TERMINAL_COLOR_SCHEME=\"${CMAKE_CURRENT_SOURCE_DIR}/src/styles/kogence-terminal.colorscheme\""
    PRIVATE
        HAVE_POSIX_OPENPT
        HAVE_SYS_TIME_H
        "KB_LAYOUT_DIR=\"${QTERMWIDGET_SOURCE_DIR}/lib/kb-layouts\""
        "COLORSCHEMES_DIR=\"${QTERMWIDGET_SOURCE_DIR}/lib/color-schemes\""
        "TRANSLATIONS_DIR=\"${QTERMWIDGET_SOURCE_DIR}/lib/translations\""
)
target_link_libraries(qtermwidget6 PUBLIC Qt6::Widgets)

add_executable(kogence-ide
    src/app/main.cpp
    src/app/mainwindow.cpp
    src/app/mainwindow.h
    src/app/thememanager.cpp
    src/app/thememanager.h
    src/app/themeselectordialog.cpp
    src/app/themeselectordialog.h
    src/panels/editor/editorpanel.cpp
    src/panels/editor/editorpanel.h
    src/panels/editor/editorpalette.h
    src/panels/editor/editortheme.h
    src/panels/editor/dartlexer.cpp
    src/panels/editor/dartlexer.h
    src/panels/editor/fileexplorerwidget.cpp
    src/panels/editor/fileexplorerwidget.h
    src/panels/terminal/terminalpanel.cpp
    src/panels/terminal/terminalpanel.h
    src/panels/preview/previewpanel.cpp
    src/panels/preview/previewpanel.h
    src/panels/kodesigner/kodesignerpanel.cpp
    src/panels/kodesigner/kodesignerpanel.h
    src/panels/kodesigner/kodesigner.qrc
    src/resources/kogence.qrc
)

target_include_directories(kogence-ide PRIVATE
    src/app
    src/panels/editor
    src/panels/preview
    src/panels/terminal
    src/panels/kodesigner
)

target_link_libraries(kogence-ide PRIVATE
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::WebChannel
    ${KOGENCE_QSCINTILLA_TARGET}
    qtermwidget6
    ads::qtadvanceddocking
)

include(GNUInstallDirs)

set(KOGENCE_DESKTOP_ID "io.github.raphaelmoyo336-spec.KogenceIDE")
install(TARGETS kogence-ide
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(FILES packaging/kogence-ide.desktop
    DESTINATION "${CMAKE_INSTALL_DATADIR}/applications"
)
install(FILES packaging/kogence-ide.png
    DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/512x512/apps"
    RENAME "${KOGENCE_DESKTOP_ID}.png"
)
install(FILES src/styles/kogence-terminal.colorscheme
    DESTINATION "${CMAKE_INSTALL_DATADIR}/kogence-ide"
)
install(FILES src/panels/kodesigner/assets/GRAPESJS-LICENSE.txt
    DESTINATION "${CMAKE_INSTALL_DATADIR}/kogence-ide/licenses"
)
install(DIRECTORY external/qtermwidget/lib/kb-layouts/
    DESTINATION "${CMAKE_INSTALL_DATADIR}/kogence-ide/kb-layouts"
)
install(DIRECTORY external/qtermwidget/lib/color-schemes/
    DESTINATION "${CMAKE_INSTALL_DATADIR}/kogence-ide/color-schemes"
)

if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()

add_executable(test_main_window
    test_main_window.cpp
    ../../src/app/mainwindow.cpp
    ../../src/app/mainwindow.h
    ../../src/app/thememanager.cpp
    ../../src/app/thememanager.h
    ../../src/app/themeselectordialog.cpp
    ../../src/app/themeselectordialog.h
    ../../src/panels/editor/editorpanel.cpp
    ../../src/panels/editor/editorpanel.h
    ../../src/panels/editor/dartlexer.cpp
    ../../src/panels/editor/dartlexer.h
    ../../src/panels/editor/fileexplorerwidget.cpp
    ../../src/panels/editor/fileexplorerwidget.h
    ../../src/panels/preview/previewpanel.cpp
    ../../src/panels/preview/previewpanel.h
    ../../src/panels/terminal/terminalpanel.cpp
    ../../src/panels/terminal/terminalpanel.h
    ../../src/panels/kodesigner/kodesignerpanel.cpp
    ../../src/panels/kodesigner/kodesignerpanel.h
    ../../src/panels/kodesigner/kodesigner.qrc
    ../../src/resources/kogence.qrc
)
target_include_directories(test_main_window PRIVATE
    ../../src/app
    ../../src/panels/editor
    ../../src/panels/preview
    ../../src/panels/terminal
    ../../src/panels/kodesigner
)
target_link_libraries(test_main_window PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::WebChannel
    Qt${QT_VERSION_MAJOR}::Test
    qtermwidget6
    ${KOGENCE_QSCINTILLA_TARGET}
    ads::qtadvanceddocking
)
add_test(NAME integration.main_window_panels
    COMMAND test_main_window createsAndTogglesTheImplementedPanels -v1
)
add_test(NAME integration.kodesigner
    COMMAND test_main_window loadsKoDesignerFromBundledAssets -v1
)
add_test(NAME integration.kodesigner_file_workflow
    COMMAND test_main_window opensAndSavesKoDesignerProjectDocuments -v1
)
add_test(NAME integration.kodesigner_html_import_export
    COMMAND test_main_window importsAndExportsExistingHtmlDesigns -v1
)
add_test(NAME integration.main_window_reattach
    COMMAND test_main_window floatingPanelCanBeReattached -v1
)
add_test(NAME regression.main_window_layout_restore
    COMMAND test_main_window savesAndRestoresDockLayout -v1
)
add_test(NAME integration.editor_shared_save
    COMMAND test_main_window editorToolbarAndNavbarShareSaveAction -v1
)
add_test(NAME integration.editor_floating_state
    COMMAND test_main_window floatingEditorPreservesItsDocument -v1
)
add_test(NAME integration.appearance_themes
    COMMAND test_main_window themeSelectorChangesApplicationAndEditorIndependently -v1
)
set_tests_properties(
    integration.main_window_panels
    integration.kodesigner
    integration.kodesigner_file_workflow
    integration.kodesigner_html_import_export
    integration.main_window_reattach
    regression.main_window_layout_restore
    integration.editor_shared_save
    integration.editor_floating_state
    integration.appearance_themes
    PROPERTIES
    LABELS "integration;ui;regression"
    TIMEOUT 60
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QTWEBENGINE_DISABLE_SANDBOX=1"
)

add_executable(test_terminal
    test_terminal.cpp
    ../../src/panels/terminal/terminalpanel.cpp
    ../../src/panels/terminal/terminalpanel.h
)
target_include_directories(test_terminal PRIVATE
    ../../src/panels/terminal
)
target_link_libraries(test_terminal PRIVATE
    Qt6::Widgets
    Qt6::Test
    qtermwidget6
)
add_test(NAME integration.terminal COMMAND test_terminal -v1)
set_tests_properties(integration.terminal PROPERTIES
    LABELS "integration"
    TIMEOUT 30
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)

add_executable(test_preview
    test_preview.cpp
    ../../src/panels/preview/previewpanel.cpp
    ../../src/panels/preview/previewpanel.h
)
target_include_directories(test_preview PRIVATE
    ../../src/panels/preview
)
target_link_libraries(test_preview PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME integration.preview COMMAND test_preview -v1)
set_tests_properties(integration.preview PROPERTIES
    LABELS "integration"
    TIMEOUT 45
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QTWEBENGINE_DISABLE_SANDBOX=1"
)

add_executable(test_editor
    test_editor.cpp
    ../../src/panels/editor/editorpanel.cpp
    ../../src/panels/editor/editorpanel.h
    ../../src/panels/editor/dartlexer.cpp
    ../../src/panels/editor/dartlexer.h
    ../../src/panels/editor/fileexplorerwidget.cpp
    ../../src/panels/editor/fileexplorerwidget.h
)
target_include_directories(test_editor PRIVATE
    ../../src/panels/editor
)
target_link_libraries(test_editor PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
    ${KOGENCE_QSCINTILLA_TARGET}
)
add_test(NAME unit.editor COMMAND test_editor -v1)
set_tests_properties(unit.editor PROPERTIES
    LABELS "unit"
    TIMEOUT 30
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)

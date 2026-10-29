add_executable(test_repeated_operations
    test_repeated_operations.cpp
    ../../src/panels/editor/editorpanel.cpp
    ../../src/panels/editor/editorpanel.h
    ../../src/panels/editor/dartlexer.cpp
    ../../src/panels/editor/dartlexer.h
    ../../src/panels/editor/fileexplorerwidget.cpp
    ../../src/panels/editor/fileexplorerwidget.h
)
target_include_directories(test_repeated_operations PRIVATE
    ../support
    ../../src/panels/editor
)
target_link_libraries(test_repeated_operations PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
    ${KOGENCE_QSCINTILLA_TARGET}
    ads::qtadvanceddocking
)
add_test(NAME stress.repeated_operations COMMAND test_repeated_operations -v1)
set_tests_properties(stress.repeated_operations PROPERTIES
    LABELS "stress;regression"
    TIMEOUT 120
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)

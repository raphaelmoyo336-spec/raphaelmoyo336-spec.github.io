execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE KOGENCE_GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT KOGENCE_GIT_REVISION)
    set(KOGENCE_GIT_REVISION "unknown")
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(KOGENCE_BENCHMARK_BUILD_TYPE "unspecified")
else()
    set(KOGENCE_BENCHMARK_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif()

add_executable(kogence_benchmarks
    benchmark_main.cpp
    ../../src/panels/editor/editorpanel.cpp
    ../../src/panels/editor/editorpanel.h
    ../../src/panels/editor/dartlexer.cpp
    ../../src/panels/editor/dartlexer.h
    ../../src/panels/editor/fileexplorerwidget.cpp
    ../../src/panels/editor/fileexplorerwidget.h
)
target_include_directories(kogence_benchmarks PRIVATE
    ../../src/panels/editor
)
target_link_libraries(kogence_benchmarks PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${KOGENCE_QSCINTILLA_TARGET}
    ads::qtadvanceddocking
)
target_compile_definitions(kogence_benchmarks PRIVATE
    KOGENCE_GIT_REVISION="${KOGENCE_GIT_REVISION}"
    KOGENCE_BUILD_TYPE="${KOGENCE_BENCHMARK_BUILD_TYPE}"
    KOGENCE_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    KOGENCE_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
)
add_test(NAME benchmark.smoke
    COMMAND kogence_benchmarks
            --iterations=5
            --warmup=1
            --output=${CMAKE_BINARY_DIR}/benchmark-results.json
)
set_tests_properties(benchmark.smoke PROPERTIES
    LABELS "benchmark"
    TIMEOUT 60
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)

add_subdirectory(unit)
add_subdirectory(integration)
add_subdirectory(stress)

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Automatically find all test-related source files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

# The test executable — includes your custom main.cpp + all test classes
add_executable(tests
    ${TEST_SOURCES}
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(tests
    PRIVATE
        app_core          # We need access to production code
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Test
)

# One single CTest entry — runs your TestRegistry which discovers everything
add_test(
    NAME AllRegisteredTests
    COMMAND tests
)

# Optional: nicer output when running ctest
set_tests_properties(AllRegisteredTests
    PROPERTIES
        LABELS "unit;auto"
)

# Optional: make "make check" or VS "Build → Run CTest" more convenient
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS tests
)
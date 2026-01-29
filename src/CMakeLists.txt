file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
)

# Exclude only what should NOT be in the library
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "main\\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "helpers\\.cpp$")
list(FILTER PROJECT_SOURCES EXCLUDE REGEX "_test\\.cpp$")

add_library(app_core STATIC ${PROJECT_SOURCES})

target_include_directories(app_core 
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(app_core
    PRIVATE 
        cxx_std_17
)

set_target_properties(app_core PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(app_core
    PRIVATE 
        SCINTILLA_QT
)

target_link_libraries(app_core PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
)

qt_add_resources(APP_RESOURCES ${RESOURCE_FILES})

add_executable(VisualLinkerScript
    main.cpp
    ${APP_RESOURCES}
)

target_link_libraries(VisualLinkerScript 
    PRIVATE 
        app_core
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
)

# Auto-copy Qt + MSVC runtime DLLs to output folder (very helpful on Windows)
if(WIN32)
    set_target_properties(VisualLinkerScript PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )

    add_custom_command(TARGET VisualLinkerScript POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:VisualLinkerScript> $<TARGET_FILE_DIR:VisualLinkerScript>
        COMMAND_EXPAND_LISTS
    )

    # Find windeployqt (usually in Qt6_DIR/bin or PATH)
    get_target_property(QT_BIN_DIR Qt${QT_VERSION_MAJOR}::qmake LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_BIN_DIR}" DIRECTORY)

    add_custom_command(TARGET VisualLinkerScript POST_BUILD
        COMMAND "${QT_BIN_DIR}/windeployqt.exe"
            --dir "$<TARGET_FILE_DIR:VisualLinkerScript>"
            --libdir "$<TARGET_FILE_DIR:VisualLinkerScript>"
            --plugindir "$<TARGET_FILE_DIR:VisualLinkerScript>/plugins"
            --no-compiler-runtime    # Skip MSVC redist if you handle it separately
            --no-translations
            "$<TARGET_FILE:VisualLinkerScript>"
        COMMENT "Running windeployqt to copy Qt plugins and DLLs (including platforms/qwindows.dll)"
    )
endif()
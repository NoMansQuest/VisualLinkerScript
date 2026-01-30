cmake_minimum_required(VERSION 3.20...3.30)
project(VisualLinkerScript LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC               ON)
set(CMAKE_AUTORCC               ON)
set(CMAKE_AUTOUIC               ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_AUTOMOC_NO_MOC_COMPILATION ON)

# Find Qt6 modules we need
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets PrintSupport Test)

# Enable CTest so we can use `ctest` / VS "Run Tests"
enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
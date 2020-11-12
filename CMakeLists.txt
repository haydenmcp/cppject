cmake_minimum_required(VERSION 3.14)
project(
    cppject
    HOMEPAGE_URL "https://github.com/haydenmcp/cppject"
    LANGUAGES CXX
    VERSION 0.0.1)
include(CMakeDependentOption)
include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON CACHE BOOL "Ensure all windows symbols are exported." FORCE)

add_library(cppject INTERFACE)

add_subdirectory("include")
add_subdirectory("test")

target_compile_features(cppject INTERFACE cxx_std_17)
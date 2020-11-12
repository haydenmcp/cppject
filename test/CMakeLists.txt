project(cppject-tests CXX)

add_executable(cppject-tests "")

message(STATUS "Adding service testing sources")
add_subdirectory("src")

message(STATUS "Handling testing dependencies")
include("extern/upstream/CMakeLists.txt")

message(STATUS "Linking testing targets")
target_link_libraries(
    cppject-tests
    PUBLIC
        cppject
        gtest gtest_main gmock
)

add_test(
    NAME cppject-tests
    COMMAND "$<TARGET_FILE:cppject-tests>"
)
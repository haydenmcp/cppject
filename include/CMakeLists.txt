add_subdirectory("cppject")

target_include_directories(cppject
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>")
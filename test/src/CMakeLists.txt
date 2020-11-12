target_sources(
    cppject-tests
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/main.cpp>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/service_factory.integration.cpp>"

        "$<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/main.cpp>"
        "$<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/service_factory.integration.cpp>"
)
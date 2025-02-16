cmake_minimum_required(VERSION 3.5)
project(CPP_Joules VERSION 0.0.1 DESCRIPTION "Energy Measurement toolkit for cpp/cuda" LANGUAGES CXX)

option(CPPJOULES_BUILD_EXAMPLES "" ON)

add_subdirectory(src)

if(CPPJOULES_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to /usr"
    )
    set(CMAKE_INSTALL_PREFIX
        "/usr"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()


# include(GNUInstallDirs)

# install(TARGETS CPP_Joules 
#         EXPORT CPP_Joules_Targets
#         PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPP_Joules
#         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(EXPORT CPP_Joules_Targets
#         FILE CPP_Joules_Targest.cmake
#         NAMESPACE CPP_Joules::
#         DESTINATION cmake
# )

# # Package configuration
# include(CMakePackageConfigHelpers)

# write_basic_package_version_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/CPP_Joules_ConfigVersion.cmake"
#   VERSION ${PROJECT_VERSION}
#   COMPATIBILITY AnyNewerVersion
# )

# configure_package_config_file(
#   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPP_Joules_Config.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/CPP_Joules_Config.cmake"
#   INSTALL_DESTINATION cmake
# )

# install(FILES
#   "${CMAKE_CURRENT_BINARY_DIR}/CPP_Joules_Config.cmake"
#   "${CMAKE_CURRENT_BINARY_DIR}/CPP_Joules_ConfigVersion.cmake"
#   DESTINATION cmake
# )


include(FetchContent)

add_library(CPP_Joules
    cppJoules.cpp
    nvidia_devices.cpp
    rapl_devices.cpp

    cppjoules/ioutils.cpp
)

target_compile_features(CPP_Joules PUBLIC cxx_std_17)
target_include_directories(CPP_Joules PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../include
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(CPP_Joules PRIVATE "-fexceptions")
endif()


##########################################################################################
# Libraries
##########################################################################################
# NVML (We load the go-nvml bindings since there does not seem to be a repo with just the header)
FetchContent_Declare(nvml GIT_REPOSITORY https://github.com/NVIDIA/go-nvml.git GIT_TAG v0.12.4-0)
FetchContent_MakeAvailable(nvml)
target_include_directories(CPP_Joules PRIVATE ${nvml_SOURCE_DIR}/gen/)

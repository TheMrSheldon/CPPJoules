include(FetchContent)

add_library(CPP_Joules
    cppJoules.cpp
    ioutils.cpp
    detail/nvmldevice.cpp
    detail/pcmdevice.cpp
    detail/rapldevice.cpp
)

target_compile_features(CPP_Joules PUBLIC cxx_std_17)
target_include_directories(CPP_Joules PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../include
)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(CPP_Joules PRIVATE "-fexceptions")
endif()

##########################################################################################
# Options
##########################################################################################
option(CPPJOULES_ENABLE_PCM "" ON)
option(CPPJOULES_ENABLE_NVML "" ON)

if((UNIX AND APPLE) OR WIN32)
    set(CPPJOULES_ENABLE_PCM OFF)
    message(INFO "pcm is not supported on macOS and Windows")
endif()

##########################################################################################
# Libraries
##########################################################################################
# NVML (We load the go-nvml bindings since there does not seem to be a repo with just the header)
FetchContent_Declare(nvml GIT_REPOSITORY https://github.com/NVIDIA/go-nvml.git GIT_TAG v0.12.4-0)
FetchContent_MakeAvailable(nvml)
target_include_directories(CPP_Joules PRIVATE ${nvml_SOURCE_DIR}/gen/)

# PCM
if(CPPJOULES_ENABLE_PCM)
    set(PCM_BUILD_EXECUTABLES OFF CACHE BOOL "" FORCE)
    set(PCM_NO_ASAN OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(pcm GIT_REPOSITORY https://github.com/intel/pcm.git GIT_TAG 202409)
    FetchContent_MakeAvailable(pcm)
    target_link_libraries(CPP_Joules PRIVATE PCM_STATIC)
    target_include_directories(CPP_Joules PRIVATE ${pcm_SOURCE_DIR}/)
    target_compile_definitions(CPP_Joules PRIVATE CPPJOULES_PCM_ENABLED)
endif()
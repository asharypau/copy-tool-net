cmake_minimum_required(VERSION 3.31)
project(CopyToolNetSln LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(STATUS "Debug build with Clang")

        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

        add_compile_options(
            -g
            -O0
            -fno-omit-frame-pointer
            -fno-inline
            -fvisibility=hidden
            -fms-compatibility
            -fsanitize=address
            -fsanitize=undefined
        )

        add_link_options(
            -fsanitize=address
            -fsanitize=undefined
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Debug build with MSVC")

        add_compile_options(
            /Od
            /MDd
            /RTC1
            /Zo
            /EHsc
            /analyze
            /sdl
            /GS
            /DEBUG
            /fsanitize=address
        )
    endif()
endif()

# Download and configure boost
include(FetchContent)

FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(Boost)

# Find OpenSSL
set(OPENSSL_ROOT_DIR "D:/Projects/External/vcpkg/installed/x64-windows")
set(OPENSSL_LIBRARIES "D:/Projects/External/vcpkg/installed/x64-windows/lib")
set(OPENSSL_INCLUDE_DIR "D:/Projects/External/vcpkg/installed/x64-windows/include")
find_package(OpenSSL REQUIRED)

add_subdirectory(src)

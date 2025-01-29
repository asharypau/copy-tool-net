cmake_minimum_required(VERSION 3.31)
project(CopyToolIpcSln LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add sanitizers for debug builds
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

#     add_compile_options("-fsanitize=address" "-g")
#     add_link_options("-fsanitize=address")

#     add_compile_options(-fsanitize=undefined)
#     add_link_options(-fsanitize=undefined)
# endif()

# Download and configure boost
include(FetchContent)

FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
    URL_MD5 893b5203b862eb9bbd08553e24ff146a
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    EXCLUDE_FROM_ALL
)

set(BOOST_INCLUDE_LIBRARIES asio)
FetchContent_GetProperties(Boost)

if(NOT Boost_POPULATED)
    FetchContent_MakeAvailable(Boost)
endif()

add_subdirectory(src)

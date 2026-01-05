cmake_minimum_required(VERSION 3.16)
project(peelmybanana LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)

add_executable(peelmybanana
    src/main.cpp
    src/utils.cpp
    src/crc32.cpp
    src/pkh.cpp
    src/pfs.cpp
    src/extractor.cpp
    src/packer.cpp
)

target_include_directories(peelmybanana PRIVATE include)
target_link_libraries(peelmybanana PRIVATE ZLIB::ZLIB)

cmake_minimum_required(VERSION 3.11)
project(tileson_tests)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/../external_libs/ ${PROJECT_SOURCE_DIR}/../src)

add_executable(tileson_tests tests_main.cpp tests_objects.cpp tests_tiled.cpp)

if(LINUX)
    message("LINUX!")
    target_link_libraries(tileson_tests tileson stdc++fs)
elseif(APPLE)
    message("OSX!")
    target_link_libraries(tileson_tests tileson /usr/local/opt/llvm/lib/libc++experimental.a /usr/local/opt/llvm/lib/libc++fs.a)
elseif(WINDOWS)
    message("WINDOWS!")
    target_link_libraries(tileson_tests tileson)
endif()
cmake_minimum_required(VERSION 3.11)
project(tileson_tests)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/../external_libs/ ${PROJECT_SOURCE_DIR}/../include)

add_executable(tileson_tests tests_main.cpp tests_objects.cpp tests_tiled_nlohmann.cpp tests_reported_issues.cpp
        tests_compression_or_encoding.cpp tests_projects_and_worlds.cpp tests_interfaces.cpp tests_tiled_picojson.cpp
        tests_benchmarks.cpp tests_tiled_gason.cpp tests_tiled_json11.cpp tests_animation.cpp)

#add_executable(tileson_tests tests_main.cpp tests_tiled_picojson.cpp)

if(LINUX)
    message("LINUX!")
    target_link_libraries(tileson_tests stdc++fs)
    #target_link_libraries(tileson_tests tileson stdc++fs)
elseif(APPLE)
    message("OSX!")
    if(CLEAR_TSON_CMAKE_CACHE)
        message("Using legacy filesystem for tests!")
        target_link_libraries(tileson_tests /usr/local/opt/llvm/lib/libc++experimental.a /usr/local/opt/llvm/lib/libc++fs.a)
    endif()
elseif(WINDOWS)
    message("WINDOWS!")
    if(MSVC)
        message("MSVC!")
        target_compile_options(tileson_tests PUBLIC /bigobj)
    else()
        message("NOT MSVC!")
        target_compile_options(tileson_tests PUBLIC)
    endif()
    #target_link_libraries(tileson_tests tileson)
endif()

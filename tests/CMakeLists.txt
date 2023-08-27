cmake_minimum_required(VERSION 3.16)
project(tileson_tests)

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTILESON_UNIT_TEST_USE_SINGLE_HEADER")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTILESON_UNIT_TEST_USE_SINGLE_HEADER")

# Uses the single header file when defined - comment out when creating PR: The cmake version in github doesn't seem to support it.
# add_compile_definitions(TILESON_UNIT_TEST_USE_SINGLE_HEADER)

message("Copying content from ${CMAKE_SOURCE_DIR}/content/test-maps to ${CMAKE_CURRENT_BINARY_DIR}")
file(COPY ${CMAKE_SOURCE_DIR}/content/test-maps DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/../external_libs/ ${PROJECT_SOURCE_DIR}/../include)

add_executable(tileson_tests tests_main.cpp tests_objects.cpp tests_tiled_nlohmann.cpp tests_reported_issues.cpp
        tests_compression_or_encoding.cpp tests_projects_and_worlds.cpp tests_interfaces.cpp tests_tiled_picojson.cpp
        tests_benchmarks.cpp tests_tiled_gason.cpp tests_tiled_json11.cpp tests_animation.cpp tests_enums_and_classes.cpp TestTools.hpp
        TestEnums.h)

#add_executable(tileson_tests tests_main.cpp tests_tiled_picojson.cpp)

if(LINUX)
    message("LINUX!")
    target_link_libraries(tileson_tests stdc++fs)
    target_compile_options(tileson_tests PRIVATE -Wall -Wextra -Werror)
elseif(APPLE)
    message("OSX!")
    target_link_libraries(tileson_tests c++)
    target_compile_options(tileson_tests PRIVATE -Wall -Wextra -Werror)
elseif(WINDOWS)
    message("WINDOWS!")
    if(MSVC)
        message("MSVC!")
        target_compile_options(tileson_tests PUBLIC /bigobj /W4 /WX)
    else()
        message("NOT MSVC!")
        target_compile_options(tileson_tests PUBLIC)
    endif()
    #target_link_libraries(tileson_tests tileson)
endif()

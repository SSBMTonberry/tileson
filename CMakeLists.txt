cmake_minimum_required(VERSION 3.11)
project(tileson)

set(CMAKE_CXX_STANDARD 17)
set(TILESON_PROJECT_VERSION_MAJOR 1)
set(TILESON_PROJECT_VERSION_MINOR 0)
set(TILESON_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTILESON_DEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
            "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG 1)
    message(DEBUGGIN')
else()
    set(RELEASE 1)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX 1)
    set(WINDOWS 0)
    set(APPLE 0)
    set(MSVC 0)
    set(MINGW 0)
elseif(APPLE)
    link_directories(/usr/local/opt/llvm/lib/)
    set(LINUX 0)
    set(WINDOWS 0)
    set(APPLE 1)
    set(MSVC 0)
    set(MINGW 0)
elseif(WIN32)
    set(LINUX 0)
    set(WINDOWS 1)
    set(APPLE 0)
    if(MSVC)
        set(MSVC 1)
        set(MINGW 0)
    elseif(MINGW)
        set(MINGW 1)
        set(MSVC 0)
    endif()
else()
    set(LINUX 0)
    set(WINDOWS 0)
    set(APPLE 0)
    set(MSVC 0)
    set(MINGW 0)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(BUILD_TESTS "Build unit tests." ON)
option(BUILD_EXAMPLES "Build examples." OFF)
if(${APPLE})
    option(USE_CPP17_FILESYSTEM "Uses cpp17 std::filesystem for anything file related" OFF)
else()
    option(USE_CPP17_FILESYSTEM "Uses cpp17 std::filesystem for anything file related" ON)
endif()

if(${USE_CPP17_FILESYSTEM})
    set(TILESON_USE_FILESYSTEM 1)
else()
    set(TILESON_USE_FILESYSTEM 0)
endif()

configure_file (
        "${PROJECT_SOURCE_DIR}/TilesonConfig.h.in"
        "${PROJECT_SOURCE_DIR}/TilesonConfig.h"
)

include_directories(${PROJECT_SOURCE_DIR}/external_libs/)

set(SOURCE_FILES src/objects/Vector2.hpp src/Tileson.cpp src/Tileson.h src/tiled/Map.cpp src/tiled/Map.h src/tiled/Layer.cpp src/tiled/Layer.h src/tiled/Chunk.cpp src/tiled/Chunk.h src/tiled/Object.cpp src/tiled/Object.h src/objects/Property.cpp src/objects/Property.h src/objects/PropertyCollection.cpp src/objects/PropertyCollection.h src/tiled/Tileset.cpp src/tiled/Tileset.h src/tiled/Tile.cpp src/tiled/Tile.h src/tiled/Frame.cpp src/tiled/Frame.h src/tiled/Terrain.cpp src/tiled/Terrain.h src/tiled/WangSet.cpp src/tiled/WangSet.h src/tiled/WangColor.cpp src/tiled/WangColor.h src/tiled/WangTile.cpp src/tiled/WangTile.h src/tiled/Text.hpp src/tiled/Grid.cpp src/tiled/Grid.h src/objects/Color.hpp src/misc/MemoryBuffer.cpp src/misc/MemoryBuffer.h src/misc/MemoryStream.cpp src/misc/MemoryStream.h)
add_library(tileson ${SOURCE_FILES})

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()

if(${BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

unset(BUILD_SHARED_LIBS CACHE) #Must clear the cache to detect changes in this variable
unset(BUILD_TESTS CACHE) #Must clear the cache to detect changes in this variable
unset(BUILD_EXAMPLES CACHE) #Must clear the cache to detect changes in this variable
unset(USE_CPP17_FILESYSTEM CACHE)
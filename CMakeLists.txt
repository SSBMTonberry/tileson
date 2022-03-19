cmake_minimum_required(VERSION 3.11)
project(tileson)

set(CMAKE_CXX_STANDARD 17)
set(TILESON_PROJECT_VERSION_MAJOR 1)
set(TILESON_PROJECT_VERSION_MINOR 4)
set(TILESON_PROJECT_VERSION_PATCH 0)

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


#option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(BUILD_TESTS "Build unit tests." ON)
option(BUILD_EXAMPLES "Build examples." OFF)
option(CLEAR_TSON_CMAKE_CACHE "Clear CMake cache for variables. Activate if you are working on the code. Otherwise: keep it off" OFF)

option(USE_LEGACY_CLANG_FOR_TESTS "Only use when you have an old version of Homebrew's Clang on OSX" OFF)

configure_file (
        "${PROJECT_SOURCE_DIR}/TilesonConfig.h.in"
        "${PROJECT_SOURCE_DIR}/TilesonConfig.h"
)

include_directories(${PROJECT_SOURCE_DIR}/external_libs/)

#set(SOURCE_FILES src/objects/Vector2.hpp src/Tileson.cpp src/Tileson.h src/tiled/Map.cpp src/tiled/Map.h src/tiled/Layer.cpp src/tiled/Layer.h src/tiled/Chunk.cpp src/tiled/Chunk.h src/tiled/Object.cpp src/tiled/Object.h src/objects/Property.cpp src/objects/Property.h src/objects/PropertyCollection.cpp src/objects/PropertyCollection.h src/tiled/Tileset.cpp src/tiled/Tileset.h src/tiled/Tile.cpp src/tiled/Tile.h src/tiled/Frame.cpp src/tiled/Frame.h src/tiled/Terrain.cpp src/tiled/Terrain.h src/tiled/WangSet.cpp src/tiled/WangSet.h src/tiled/WangColor.cpp src/tiled/WangColor.h src/tiled/WangTile.cpp src/tiled/WangTile.h src/tiled/Text.hpp src/tiled/Grid.cpp src/tiled/Grid.h src/objects/Color.hpp src/misc/MemoryBuffer.cpp src/misc/MemoryBuffer.h src/misc/MemoryStream.cpp src/misc/MemoryStream.h include/tileson_parser.hpp)
set(INCLUDE_FILES include/tileson.h include/tileson_parser.hpp include/misc/MemoryBuffer.hpp include/misc/MemoryStream.hpp include/objects/Color.hpp
        include/objects/Property.hpp include/objects/PropertyCollection.hpp include/objects/Vector2.hpp include/tiled/Chunk.hpp include/tiled/Frame.hpp
        include/tiled/Grid.hpp include/tiled/Layer.hpp include/tiled/Map.hpp include/tiled/Object.hpp include/tiled/Terrain.hpp include/tiled/Text.hpp
        include/tiled/Tile.hpp include/tiled/Tileset.hpp include/tiled/WangColor.hpp include/tiled/WangSet.hpp include/tiled/WangTile.hpp
        include/objects/Rect.hpp include/common/tileson_forward.hpp include/interfaces/IDecompressor.hpp include/common/Base64Decompressor.hpp
        include/common/DecompressorContainer.hpp include/common/Tools.hpp include/tiled/Project.hpp include/objects/ProjectData.hpp
        include/objects/ProjectFolder.hpp include/tiled/World.hpp include/objects/WorldMapData.hpp)

# Only activated for code completion, must be uncommented before release!
# add_library(tileson ${SOURCE_FILES} ${INCLUDE_FILES})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    set(BUILD_SHARED_LIBS OFF) #build SFML with static libs
    #set(SFML_BUILD_AUDIO OFF)

    add_subdirectory(external_libs/source/sfml)
    add_subdirectory(examples)
endif()

if(CLEAR_TSON_CMAKE_CACHE)
    unset(BUILD_TESTS CACHE) #Must clear the cache to detect changes in this variable
    unset(BUILD_EXAMPLES CACHE) #Must clear the cache to detect changes in this variable
    unset(DISABLE_CPP17_FILESYSTEM CACHE)
endif()
unset(CLEAR_TSON_CMAKE_CACHE CACHE)
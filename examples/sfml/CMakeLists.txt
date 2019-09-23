cmake_minimum_required(VERSION 3.11)
project(tileson-sfml-example)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -DSFML_STATIC) #required to build SFML statically with Windows

set(SOURCE_FILES main.cpp SfmlDemoManager.cpp SfmlDemoManager.h)

if(LINUX)
    add_executable(tileson-sfml-example ${SOURCE_FILES})
    target_link_libraries(tileson-sfml-example tileson stdc++fs png ${MY_STATIC_LIBS} ${SFML_DEPENDENCIES} ${SFML_REST})
elseif(WIN32)
    if(MSVC)
        add_executable(TilesonSfmlExample ${SOURCE_FILES}) #${PROJECT_SOURCE_DIR}../logo/icon.rc)
        target_link_libraries(TilesonSfmlExample tileson ${MY_STATIC_LIBS} ${SFML_DEPENDENCIES})
    endif()
elseif(APPLE)
    set(FRAMEWORK_LIBS_BASE "-framework Carbon" "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" "-framework CoreFoundation")

    set(MACOSX_BUNDLE_BUNDLE_NAME TilesonSfmlExample)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "tileson.sfml.example")
    set(MACOSX_BUNDLE_INFO_STRING "Tileson SFML Example")
    set(MACOSX_BUNDLE_ICON_FILE "AppIcon.icns")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING ${TILESON_PROJECT_VERSION_MAJOR}.${TILESON_PROJECT_VERSION_MINOR}.${TILESON_PROJECT_VERSION_PATCH})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${TILESON_PROJECT_VERSION_MAJOR}.${TILESON_PROJECT_VERSION_MINOR}.${TILESON_PROJECT_VERSION_PATCH})
    set(MACOSX_BUNDLE_BUNDLE_VERSION ${TILESON_PROJECT_VERSION_MAJOR}.${TILESON_PROJECT_VERSION_MINOR}.${TILESON_PROJECT_VERSION_PATCH})

    #set_source_files_properties(${PROJECT_SOURCE_DIR}../logo/AppIcon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    add_executable(TilesonSfmlExample MACOSX_BUNDLE ${SOURCE_FILES})
    #stdc++
    if(SFML_BUILD_STATIC)
        target_link_libraries(TilesonSfmlExample tileson stdc++ -ObjC /usr/local/opt/llvm/lib/libc++experimental.a /usr/local/opt/llvm/lib/libc++fs.a ${MY_STATIC_LIBS} ${FRAMEWORK_LIBS_BASE} ${SFML_DEPENDENCIES})
    else()
        target_link_libraries(TilesonSfmlExample tileson stdc++ -ObjC /usr/local/opt/llvm/lib/libc++experimental.a /usr/local/opt/llvm/lib/libc++fs.a ${MY_DYNAMIC_LIBS} ${FRAMEWORK_LIBS_BASE} ${SFML_DEPENDENCIES})
        set_target_properties(TilesonSfmlExample PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
                FRAMEWORK ${MY_DYNAMIC_LIBS})

    endif()
endif()

#add_executable(tileson-sfml-example ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.16)
project(tileson-sfml-example)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -DSFML_STATIC) #required to build SFML statically with Windows

set(IMGUI_PATH ${PROJECT_SOURCE_DIR}/../../external_libs/source/imgui/)
set(SOURCE_FILES main.cpp SfmlDemoManager.cpp SfmlDemoManager.h ${IMGUI_PATH}/imgui.cpp ${IMGUI_PATH}/imgui_draw.cpp ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui-SFML.cpp ${IMGUI_PATH}/imgui_widgets.cpp)
set(MY_SFML_LIBS sfml-network sfml-audio sfml-graphics sfml-window sfml-system)

if(LINUX)
    add_executable(tileson-sfml-example ${SOURCE_FILES})
    target_link_libraries(tileson-sfml-example stdc++fs png ${MY_SFML_LIBS} )
elseif(WIN32)
    if(MSVC)
        add_executable(TilesonSfmlExample ${SOURCE_FILES})
        target_link_libraries(TilesonSfmlExample ${MY_SFML_LIBS} )
    else()
        #Presume something like MinGW
        add_executable(TilesonSfmlExample ${SOURCE_FILES})
        target_link_libraries(TilesonSfmlExample stdc++fs png ${MY_SFML_LIBS})
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
    if(SFML_BUILD_STATIC)
        target_link_libraries(TilesonSfmlExample c++ ${MY_SFML_LIBS} ${FRAMEWORK_LIBS_BASE} )
    else()
        target_link_libraries(TilesonSfmlExample c++ ${MY_SFML_LIBS} ${FRAMEWORK_LIBS_BASE} )
        set_target_properties(TilesonSfmlExample PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
                FRAMEWORK ${MY_SFML_LIBS})

    endif()
endif()

#add_executable(tileson-sfml-example ${SOURCE_FILES})
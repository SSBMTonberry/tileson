//
// Created by robin on 03.08.2020.
//

#ifndef DISABLE_CPP17_FILESYSTEM
#include "../external_libs/catch.hpp"
#include "tileson.h"

TEST_CASE( "Parse world - Expect 4 maps", "[project][world]" )
{
    fs::path pathLocal {"../../content/test-maps/project/world/test.world"};
    fs::path pathTravis {"../content/test-maps/project/world/test.world"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::World world {pathToUse};
    REQUIRE(world.getMapData().size() == 4);
}

TEST_CASE( "Parse project - expect no crash", "[project][world]" )
{
    fs::path pathLocal {"../../content/test-maps/project/test.tiled-project"};
    fs::path pathTravis {"../content/test-maps/project/test.tiled-project"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::Project project {pathToUse};
    auto files = project.getFolders().at(0).getSubFolders().at(0).getFiles();
    std::string str;
    std::for_each(files.begin(), files.end(), [&](const auto &item) { str.append(item.u8string()); });

    REQUIRE(project.getData().folders.size() == 2);
    REQUIRE(project.getFolders().size() == 2);
    REQUIRE(project.getFolders().at(0).getPath().filename() == "maps");
    REQUIRE(!project.getFolders().at(0).hasWorldFile());
    REQUIRE(project.getFolders().at(0).getFiles().size() == 3);
    REQUIRE(project.getFolders().at(0).getSubFolders().size() == 1);
    REQUIRE(project.getFolders().at(0).getSubFolders().at(0).getFiles().size() == 1);
    REQUIRE(project.getFolders().at(1).getPath().filename() == "world");
    REQUIRE(project.getFolders().at(1).hasWorldFile());
    REQUIRE(project.getFolders().at(1).getFiles().size() == 4);
    REQUIRE(project.getFolders().at(1).getSubFolders().empty());
}

#endif
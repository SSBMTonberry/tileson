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
    REQUIRE(project.getData().folders.size() == 2);
}

#endif
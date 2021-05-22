//
// Created by robin on 22.05.2021.
//

#include <ostream>
#include "../external_libs/catch.hpp"
//#include "../include/objects/Color.hpp"
#include "../TilesonConfig.h"
//#include "../single_include/tileson.hpp"
#include "../include/tileson.h"

TEST_CASE( "Create simple animation with 3 frames. Expect right values after update", "[animation]" )
{
    tson::Animation a {{{100, 1}, {200, 2}, {100, 3}}};
    a.update(99);
    REQUIRE(a.getCurrentFrameNumber() == 0);
    REQUIRE(a.getCurrentFrame()->getDuration() == 100);
    REQUIRE(a.getCurrentFrame()->getTileId() == 1);
    REQUIRE(a.getTimeDelta() == 99);

    a.update(6);
    REQUIRE(a.getCurrentFrameNumber() == 1);
    REQUIRE(a.getCurrentFrame()->getDuration() == 200);
    REQUIRE(a.getCurrentFrame()->getTileId() == 2);
    REQUIRE(a.getTimeDelta() == 5);

    a.update(195);
    REQUIRE(a.getCurrentFrameNumber() == 2);
    REQUIRE(a.getCurrentFrame()->getDuration() == 100);
    REQUIRE(a.getCurrentFrame()->getTileId() == 3);
    REQUIRE(a.getTimeDelta() == 0);

    a.update(100);
    REQUIRE(a.getCurrentFrameNumber() == 0);
    REQUIRE(a.getCurrentFrame()->getTileId() == 1);
    REQUIRE(a.getTimeDelta() == 0);
}
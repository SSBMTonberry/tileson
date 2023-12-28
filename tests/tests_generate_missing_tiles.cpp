#include "../external_libs/catch.hpp"
#include "../TilesonConfig.h"

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
#include "../tileson.hpp"
#else
#include "../include/tileson.h"
#endif

#include "tson_files_mapper.h"
#include "../TilesonConfig.h"
#include "TestTools.hpp"

TEST_CASE("Parse map and verify that the local id is not being assigned the global id")
{
    tson::Tileson t;
    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/issues/issue_100.json"));

    std::unique_ptr<tson::Map> map = t.parse({ pathToUse });
    REQUIRE(map->getStatus() == tson::ParseStatus::OK);
    
    // The second tileset is used to ensure the local id and global id is different 
    tson::Tileset* tileset = map->getTileset("tileset_2");
    REQUIRE(tileset->getFirstgid() == 3);
        
    std::vector<tson::Tile>& tiles = tileset->getTiles();
    REQUIRE(tiles.size() == 2);

    tson::Tile* tile0 = tileset->getTile(1);
    tson::Tile* tile1 = tileset->getTile(2);

    // 1 != 3
    REQUIRE(tile0->getId() == 1);
    REQUIRE(tile0->getGid() == 3);

    // 2 != 4
    REQUIRE(tile1->getId() == 2);
    REQUIRE(tile1->getGid() == 4); 
}

TEST_CASE("Parse map and verify that animation frames do not return a null pointer")
{
    tson::Tileson t;
    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/issues/issue_110.json"));

    std::unique_ptr<tson::Map> map = t.parse({ pathToUse });
    REQUIRE(map->getStatus() == tson::ParseStatus::OK);
    
    tson::Tileset* tileset = map->getTileset("tileset_3");
    REQUIRE(tileset->getFirstgid() == 5);

    std::vector<tson::Tile>& tiles = tileset->getTiles();
    REQUIRE(tiles.size() == 2);
        
    tson::Tile* tile = tileset->getTile(2);
    REQUIRE(tile->getAnimation().any());

    for (const tson::Frame& frame : tile->getAnimation().getFrames())
    {
        REQUIRE(tileset->getTile(frame.getTileId()) != nullptr);
    }
}
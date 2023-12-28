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

TEST_CASE("Parse map and verify that the local id is not being assigned the global id for flipped tiles")
{
    tson::Tileson t;
    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/issues/issue_113.json"));

    std::unique_ptr<tson::Map> map = t.parse({ pathToUse });
    REQUIRE(map->getStatus() == tson::ParseStatus::OK);

    tson::Layer* layer = map->getLayer("Tile Layer");    
    
    tson::Tile* flipped_tile = layer->getTileData(0, 0);        
    REQUIRE(flipped_tile->getFlipFlags() != tson::TileFlipFlags::None);
    
    // The second tileset is used to ensure the Local ID and Global ID is different 
    tson::Tileset* tileset = flipped_tile->getTileset();
    REQUIRE(tileset->getFirstgid() != 1);

    // Fix
    tson::Tile* original_tile = tileset->getTile(flipped_tile->getId());
    
    REQUIRE(original_tile != flipped_tile);
    REQUIRE(original_tile->getId() == flipped_tile->getId());
    REQUIRE(original_tile->getGid() == flipped_tile->getGid());
}
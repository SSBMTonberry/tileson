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

TEST_CASE("Parse map and verify that image collection tileset data is correct")
{
    tson::Tileson t;    
    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/feature_111_demo_map.json"));

    std::unique_ptr<tson::Map> map = t.parse({ pathToUse });
    REQUIRE(map->getStatus() == tson::ParseStatus::OK);

    // Test tileset types
    tson::Tileset* imageTileset = map->getTileset("image-tileset");
    REQUIRE(imageTileset->getColumns() > 0);
    REQUIRE(imageTileset->getType() == tson::TilesetType::ImageTileset);

    tson::Tileset* imageCollectionTileset = map->getTileset("image-collection-tileset");
    REQUIRE(imageCollectionTileset->getColumns() == 0);
    REQUIRE(imageCollectionTileset->getType() == tson::TilesetType::ImageCollectionTileset);

    // Test tile dimensions
    tson::Tile* tile = imageCollectionTileset->getTile(2);
    REQUIRE(tile->getImageSize().x == 24);
    REQUIRE(tile->getImageSize().y == 8);

    // Test tile position - come back to when object layer is complete
    tson::Layer* layer = map->getLayer("Tile Layer");
    tson::TileObject* tileObject = layer->getTileObject(1, 1);
    REQUIRE(tileObject != nullptr);
    REQUIRE(tileObject->getPosition().x == 16);
    REQUIRE(tileObject->getPosition().y == 16);
}
//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/catch.hpp"
#include "../src/Tileson.h"

#include "tson_files_mapper.h"

bool mapIsAbsolutelyFine(tson::Map &map)
{
    return (map.getLayers().size() == 6 &&
            !map.isInfinite() &&
            map.getSize() == tson::Vector2i(32, 16) &&
            map.getBackgroundColor() == tson::Colori("#3288c1") &&
            map.getLayers()[0].getProperties().getSize() > 0 &&
            map.getLayers()[2].getName() == "Object Layer" &&
            map.getLayers()[2].getObjects().size() > 1 &&
            map.getLayers()[2].getObjects()[0].getName() == "coin" &&
            map.getLayers()[2].getObjects()[0].getProperties().getSize() > 0 &&
            map.getLayer("Main Layer") != nullptr &&
            map.getLayer("Main Layer")->getType() == tson::Layer::Type::TileLayer &&
            map.getLayer("Background Image")->getType() == tson::Layer::Type::ImageLayer &&
            map.getLayer("Background Image")->get<float>("scroll_speed") == 1.f &&
            map.getLayer("Background Image")->getProp("repeat_bg")->getType() == tson::Property::Type::Boolean &&
            map.getLayer("Object Layer")->getType() == tson::Layer::Type::ObjectGroup &&
            map.getTileset("demo-tileset") != nullptr &&
            map.getTileset("demo-tileset")->getTile(36) != nullptr &&
            map.getTileset("demo-tileset")->getTile(36)->getAnimation().size() == 2 &&
            map.getTileset("demo-tileset")->getTerrain("test_terrain")->getProperties().getSize() == 2 &&
            map.getTileset("demo-tileset")->getTerrain("test_terrain")->getProp("i_like_this")->getType() == tson::Property::Type::Boolean &&
            !map.getTileset("demo-tileset")->getTerrain("test_terrain")->get<std::string>("description").empty()
            );
}


TEST_CASE( "Parse a whole map by file", "[complete][parse][file]" ) {
    tson::Tileson t;
    tson::Map map = t.parse({"../../content/test-maps/ultimate_test.json"});
    REQUIRE( mapIsAbsolutelyFine(map) );
}

TEST_CASE( "Parse a whole map by memory", "[complete][parse][memory]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);
    REQUIRE( mapIsAbsolutelyFine(map) );
}
//
// Created by robin on 31.07.2019.
//

#include <tiled/Map.h>
#include <tiled/Layer.h>
#include "../external_libs/catch.hpp"

TEST_CASE( "Parse a tson::Map only containing info from Tiled's documentation - read only primitive values - expect success", "[tiled][map]" ) {

    nlohmann::json j = "{\n"
                       "  \"backgroundcolor\":\"#656667\",\n"
                       "  \"height\":4,\n"
                       "  \"layers\":[ ],\n"
                       "  \"nextobjectid\":1,\n"
                       "  \"orientation\":\"orthogonal\",\n"
                       "  \"properties\":[\n"
                       "    {\n"
                       "      \"name\":\"mapProperty1\",\n"
                       "      \"type\":\"one\",\n"
                       "      \"value\":\"string\"\n"
                       "    },\n"
                       "    {\n"
                       "      \"name\":\"mapProperty2\",\n"
                       "      \"type\":\"two\",\n"
                       "      \"value\":\"string\"\n"
                       "    }],\n"
                       "  \"renderorder\":\"right-down\",\n"
                       "  \"tileheight\":32,\n"
                       "  \"tilesets\":[ ],\n"
                       "  \"tilewidth\":32,\n"
                       "  \"version\":1,\n"
                       "  \"tiledversion\":\"1.0.3\",\n"
                       "  \"width\":4\n"
                       "}"_json;

    tson::Map map;
    bool parseOk = map.parse(j);
    bool hasCorrectValues = (
                                map.getBackgroundColor() == "#656667" &&
                                map.getSize() == tson::Vector2i(4,4) &&
                                map.getNextObjectId() == 1 &&
                                map.getOrientation() == "orthogonal" &&
                                map.getRenderOrder() == "right-down" &&
                                map.getTileSize() == tson::Vector2i(32, 32) &&
                                map.getVersion() == 1 &&
                                map.getTiledVersion() == "1.0.3"

                            );

    REQUIRE( (parseOk && hasCorrectValues) );
}

TEST_CASE( "Parse a tson::Layer only containing info from Tiled's documentation - read only primitive values - expect success", "[tiled][layer]" ) {

    nlohmann::json j = "{\n"
                       "  \"data\":[1, 2, 1, 2, 3, 1, 3, 1, 2, 2, 3, 3, 4, 4, 4, 1],\n"
                       "  \"height\":4,\n"
                       "  \"name\":\"ground\",\n"
                       "  \"opacity\":1,\n"
                       "  \"properties\":[\n"
                       "    {\n"
                       "      \"name\":\"tileLayerProp\",\n"
                       "      \"type\":\"int\",\n"
                       "      \"value\":1\n"
                       "    }],\n"
                       "  \"type\":\"tilelayer\",\n"
                       "  \"visible\":true,\n"
                       "  \"width\":4,\n"
                       "  \"x\":0,\n"
                       "  \"y\":0\n"
                       "}"_json;

    tson::Layer layer;
    bool parseOk = layer.parse(j);
    bool hasCorrectValues = (
            layer.getData().size() == 16 &&
            layer.getSize() == tson::Vector2i(4, 4) &&
            layer.getName() == "ground" &&
            layer.getOpacity() == 1 &&
            layer.getType() == "tilelayer" &&
            layer.isVisible() &&
            layer.getX() == 0 &&
            layer.getY() == 0
    );

    REQUIRE( (parseOk && hasCorrectValues) );
}
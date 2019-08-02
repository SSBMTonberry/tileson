//
// Created by robin on 31.07.2019.
//

#include <tiled/Map.h>
#include <tiled/Layer.h>
#include <tiled/Chunk.h>
#include <iostream>
#include <tiled/Object.h>
#include "../external_libs/catch.hpp"

TEST_CASE( "Parse a Map from Tiled's documentation - read simple values", "[tiled][map]" )
{

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

TEST_CASE( "Parse a Layer from Tiled's documentation - read simple values", "[tiled][layer]" )
{

    SECTION("Layer - Tile Layer format")
    {
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


        REQUIRE((parseOk && hasCorrectValues));
    }

    SECTION("Layer - Object Layer format")
    {
        nlohmann::json j = "{\n"
                           "  \"draworder\":\"topdown\",\n"
                           "  \"height\":8,\n"
                           "  \"name\":\"people\",\n"
                           "  \"objects\":[ ],\n"
                           "  \"opacity\":1,\n"
                           "  \"properties\":[\n"
                           "    {\n"
                           "      \"name\":\"layerProp1\",\n"
                           "      \"type\":\"string\",\n"
                           "      \"value\":\"someStringValue\"\n"
                           "    }],\n"
                           "  \"type\":\"objectgroup\",\n"
                           "  \"visible\":true,\n"
                           "  \"width\":4,\n"
                           "  \"x\":0,\n"
                           "  \"y\":0\n"
                           "}"_json;

        tson::Layer layer;
        bool parseOk = layer.parse(j);
        bool hasCorrectValues = (
                layer.getDrawOrder() == "topdown" &&
                layer.getName() == "people" &&
                layer.getOpacity() == 1 &&
                layer.getType() == "objectgroup" &&
                layer.isVisible() &&
                layer.getSize() == tson::Vector2i(4, 8) &&
                layer.getX() == 0 &&
                layer.getY() == 0
        );


        REQUIRE((parseOk && hasCorrectValues));
    }
}

TEST_CASE( "Parse a Chunk from Tiled's documentation - read simple values", "[tiled][chunk]" )
{
    nlohmann::json j = "{\n"
                       "  \"data\":[1, 2, 1, 2, 3, 1, 3, 1, 2, 2, 3, 3, 4, 4, 4, 1],\n"
                       "  \"height\":16,\n"
                       "  \"width\":16,\n"
                       "  \"x\":4,\n"
                       "  \"y\":-16\n"
                       "}"_json;

    tson::Chunk chunk;
    bool parseOk = chunk.parse(j);
    bool hasCorrectValues = (
            chunk.getData().size() == 16 &&
            chunk.getSize() == tson::Vector2i(16, 16) &&
            chunk.getPosition() == tson::Vector2i(4, -16)
    );

    REQUIRE((parseOk && hasCorrectValues));
}

TEST_CASE( "Parse an Object from Tiled's documentation - read simple values", "[tiled][chunk]" )
{
    SECTION("Object - regular")
    {
        nlohmann::json j = "{\n"
                           "  \"gid\":5,\n"
                           "  \"height\":0,\n"
                           "  \"id\":1,\n"
                           "  \"name\":\"villager\",\n"
                           "  \"properties\":[\n"
                           "    {\n"
                           "      \"name\":\"hp\",\n"
                           "      \"type\":\"int\",\n"
                           "      \"value\":12\n"
                           "    }],\n"
                           "  \"rotation\":90,\n"
                           "  \"type\":\"npc\",\n"
                           "  \"visible\":true,\n"
                           "  \"width\":0,\n"
                           "  \"x\":32,\n"
                           "  \"y\":32\n"
                           "}"_json;

        tson::Object obj;
        bool parseOk = obj.parse(j);
        bool hasCorrectValues = (
                obj.getGid() == 5 &&
                obj.getId() == 1 &&
                obj.getName() == "villager" &&
                obj.getRotation() == 90 &&
                obj.getType() == "npc" &&
                obj.isVisible() &&
                obj.getSize() == tson::Vector2i(0, 0) &&
                obj.getPosition() == tson::Vector2i(32, 32)
                && obj.getObjectType() == tson::Object::Type::Object
        );

        REQUIRE((parseOk && hasCorrectValues));
    }
}
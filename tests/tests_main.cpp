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
            map.getLayers()[0].getProperties().get()[0]->getType() != tson::Property::Type::Undefined &&
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
            map.getLayer("Object Layer")->firstObj("coin") != nullptr &&
            map.getLayer("Object Layer")->getObjectsByName("goomba").size() == 2 &&
            map.getLayer("Object Layer")->getObjectsByType(tson::Object::Type::Object).size() > 0 &&
            map.getLayer("Object Layer")->getObj(2)->getName() == "coin" &&
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

TEST_CASE( "Go through demo code - get success", "[demo]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);

    //Get color as an rgba color object
    tson::Colori bgColor = map.getBackgroundColor(); //RGBA with 0-255 on each channel

    //This color can be compared with Tiled hex string
    if(bgColor == "#ffaa07")
        printf("Cool!");

    //Or you can compare them with other colors
    tson::Colori otherColor {128, 87, 65, 255};
    if(bgColor == otherColor)
        printf("This works, too!");

    //You can also get the color as float, transforming values if they are already in their int form, from max 255 to 1.f
    tson::Colorf bgColorFloat = bgColor.asFloat();

    //Or the other way around
    tson::Colori newBg = bgColorFloat.asInt();

    //You can loop through every container of objects
    for(auto &layer : map.getLayers())
    {
        if(layer.getType() == tson::Layer::Type::ObjectGroup)
        {
            for(auto &obj : layer.getObjects())
            {
                //Just iterate through all the objects
            }
            //Or use these queries:

            //Gets the first object it find with the name specified
            tson::Object *player = layer.firstObj("player");

            //Gets all objects with a matching name
            std::vector<tson::Object> enemies = layer.getObjectsByName("goomba");

            //Gets all objects of a specific type
            std::vector<tson::Object> objects = layer.getObjectsByType(tson::Object::Type::Object);

            //Gets an unique object by its name.
            tson::Object *uniqueObj = layer.getObj(2);
        }
    }

    //Or get a specific object if you know its name (or id)
    tson::Layer *layer = map.getLayer("Main Layer");
    tson::Tileset *tileset = map.getTileset("demo-tileset");
    tson::Tile *tile = tileset->getTile(1); //Tileson tile-IDs starts with 1, to be consistent with IDs in data lists.


    //If an object supports properties, you can easily get a property value by calling get<T>() or the property itself with getProp()
    int myInt = layer->get<int>("my_int");
    float myFloat = layer->get<float>("my_float");
    bool myBool = layer->get<bool>("my_bool");
    std::string myString = layer->get<std::string>("my_string");
    tson::Colori myColor = layer->get<tson::Colori>("my_color");
    fs::path file = layer->get<fs::path>("my_file");
    tson::Property *prop = layer->getProp("my_property");

    REQUIRE( true );
}
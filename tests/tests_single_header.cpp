//
// Created by robin on 22.03.2020.
//


/*!
 * Single include is a bit heavy for CLion in the test project, so it is ignored unless manually changed for testing.
 */
#if 0

#include "../external_libs/catch.hpp"

#include "../TilesonConfig.h"
#include "../single_include/tileson.hpp"
//#include "../src/Tileson.h"

#include "tson_files_mapper.h"
#include "../TilesonConfig.h"

#include <memory>
#include <map>
#include <functional>

std::string getFailingTestsMessage2(const std::vector<int> &failingTests)
{
    if(failingTests.empty())
        return "";

    std::string msg = "Tests failed (by ID): ";
    bool first = true;
    for(const auto &id : failingTests)
    {
        if(!first)
            msg.append(", ");
        msg.append(std::to_string(id));
        first = false;
    }
    return msg;
}

std::vector<int> getFailingMapTests2(tson::Map *map)
{
    std::vector<int> failingTests;
    std::map<int, bool> tests;
    auto main = map->getLayer("Main Layer");
    auto tileData = main->getTileData();
    tson::Rect rect = main->getTileData(8,14)->getDrawingRect();

    tests[0] = map->getLayers().size() == 6;
    tests[1] = !map->isInfinite();
    tests[2] = map->getSize() == tson::Vector2i(32, 16);
    tests[3] = map->getBackgroundColor() == tson::Colori("#3288c1");
    tests[4] = map->getLayers()[0].getProperties().getSize() > 0;
    tests[5] = map->getLayers()[0].getProperties().get()[0]->getType() != tson::Type::Undefined;
    tests[6] = map->getLayers()[2].getName() == "Object Layer";
    tests[7] = map->getLayers()[2].getObjects().size() > 1;
    tests[8] = map->getLayers()[2].getObjects()[0].getName() == "coin";
    tests[9] = map->getLayers()[2].getObjects()[0].getProperties().getSize() > 0;
    tests[10] = map->getLayer("Main Layer") != nullptr;
    tests[11] = map->getLayer("Main Layer")->getType() == tson::LayerType::TileLayer;
    tests[12] = map->getLayer("Background Image")->getType() == tson::LayerType::ImageLayer;
    tests[13] = map->getLayer("Background Image")->get<float>("scroll_speed") == 1.f;
    tests[14] = map->getLayer("Background Image")->getProp("repeat_bg")->getType() == tson::Type::Boolean;
    tests[15] = map->getLayer("Object Layer")->getType() == tson::LayerType::ObjectGroup;
    tests[16] = map->getLayer("Object Layer")->firstObj("coin") != nullptr;
    tests[17] = map->getLayer("Object Layer")->getObjectsByName("goomba").size() == 2;
    tests[18] = !map->getLayer("Object Layer")->getObjectsByType(tson::ObjectType::Object).empty();
    tests[19] = map->getLayer("Object Layer")->getObj(2)->getName() == "coin";
    tests[20] = map->getTileset("demo-tileset") != nullptr;
    tests[21] = map->getTileset("demo-tileset")->getTile(36) != nullptr;
    tests[22] = map->getTileset("demo-tileset")->getTile(36)->getAnimation().size() == 2;
    tests[23] = map->getTileset("demo-tileset")->getTerrain("test_terrain")->getProperties().getSize() == 2;
    tests[24] = map->getTileset("demo-tileset")->getTerrain("test_terrain")->getProp("i_like_this")->getType() == tson::Type::Boolean;
    tests[25] = !map->getTileset("demo-tileset")->getTerrain("test_terrain")->get<std::string>("description").empty();
    tests[26] = map->getTileMap().size() > 10;
    tests[27] = tileData[{4,4}] != nullptr && tileData[{4,4}]->getId() == 1;
    tests[28] = tileData[{5,4}] != nullptr && tileData[{5,4}]->getId() == 3;
    tests[29] = main->getTileData(8,14) != nullptr && main->getTileData(8,14)->getId() == 2;
    tests[30] = main->getTileData(17,5) != nullptr && main->getTileData(17,5)->getId() == 5;
    //v1.2.0-tests
    tests[31] = main->getTileData(8,14)->getPositionInTileUnits({8,14}) == tson::Vector2i(8, 14);
    tests[32] = main->getTileData(8,14)->getPosition({8,14}) == tson::Vector2f(8.f * 16.f, 14.f * 16.f);
    tests[33] = main->getMap() != nullptr;
    tests[34] = main->getMap() == map;
    tests[35] = main->getTileData(8,14)->getMap() != nullptr;
    tests[36] = main->getTileData(8,14)->getMap() == map;
    tests[37] = main->getTileData(8,14)->getTileset() != nullptr;

    for(const auto & [id, result] : tests)
    {
        if(!result)
            failingTests.push_back(id);
    }

    return failingTests;
}


TEST_CASE( "Parse a whole map by file (Single Header)", "[complete][parse][file]" ) {
    tson::Tileson t;
    #ifndef DISABLE_CPP17_FILESYSTEM
    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;
    #else
    std::string pathToUse = "../../content/test-maps/ultimate_test.json";
    #endif
    std::unique_ptr<tson::Map> map = t.parse({pathToUse});
    if(map->getStatus() == tson::ParseStatus::OK)
    {
        std::vector<int> failing = getFailingMapTests2(map.get());
        if(!failing.empty())
            FAIL(getFailingTestsMessage2(failing));
        REQUIRE(failing.empty());
    }
    else
    {
        std::cout << "Ignored - " << map->getStatusMessage() << std::endl;
        REQUIRE(true);
    }
}

TEST_CASE( "Parse a whole map by memory (Single Header)", "[complete][parse][memory]" ) {
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        std::vector<int> failing = getFailingMapTests2(map.get());
        if(!failing.empty())
            FAIL(getFailingTestsMessage2(failing));
        REQUIRE(failing.empty());
    }
    else
    {
        std::cout << "Memory parse error - " << map->getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}

TEST_CASE( "Parse a simple map by memory - tiles without any properties (issue #4) (Single Header)", "[simple][parse][memory]" ) {
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(tson_files_mapper::_SIMPLE_MAP_JSON, tson_files_mapper::_SIMPLE_MAP_JSON_SIZE);
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        auto main = map->getLayer("simple_layer");
        auto tileset = map->getTilesets()[0];
        auto tileData = main->getTileData();
        bool result = (tileData.size() > 16 && main->getTileData(0,0) != nullptr && tileset.getTiles().size() == 48);
        REQUIRE(result);
    }
    else
    {
        std::cout << "Memory parse error - " << map->getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}

TEST_CASE( "Parse a minimal version of whole map by memory (Single Header)", "[complete][parse][memory]" ) {
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(tson_files::_ULTIMATE_TEST_MIN_JSON, tson_files::_ULTIMATE_TEST_MIN_JSON_SIZE);
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        std::vector<int> failing = getFailingMapTests2(map.get());
        if(!failing.empty())
            FAIL(getFailingTestsMessage2(failing));
        REQUIRE(failing.empty());
    }
    else
    {
        std::cout << "Memory parse error - " << map->getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}



TEST_CASE( "Go through demo code - get success (Single Header)", "[demo]" ) {
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);

    if(map->getStatus() == tson::ParseStatus::OK)
    {
        //Get color as an rgba color object
        tson::Colori bgColor = map->getBackgroundColor(); //RGBA with 0-255 on each channel

        //This color can be compared with Tiled hex string
        if (bgColor == "#ffaa07")
            printf("Cool!");

        //Or you can compare them with other colors
        tson::Colori otherColor{128, 87, 65, 255};
        if (bgColor == otherColor)
            printf("This works, too!");

        //You can also get the color as float, transforming values if they are already in their int form, from max 255 to 1.f
        tson::Colorf bgColorFloat = bgColor.asFloat();

        //Or the other way around
        tson::Colori newBg = bgColorFloat.asInt();

        //You can loop through every container of objects
        for (auto &layer : map->getLayers())
        {
            if (layer.getType() == tson::LayerType::ObjectGroup)
            {
                for (auto &obj : layer.getObjects())
                {
                    //Just iterate through all the objects
                }
                //Or use these queries:

                //Gets the first object it find with the name specified
                tson::Object *player = layer.firstObj("player"); //Does not exist in demo map->..

                //Gets all objects with a matching name
                std::vector<tson::Object> enemies = layer.getObjectsByName("goomba"); //But we got two of those

                //Gets all objects of a specific type
                std::vector<tson::Object> objects = layer.getObjectsByType(tson::ObjectType::Object);

                //Gets an unique object by its name.
                tson::Object *uniqueObj = layer.getObj(2);
            }
        }

        //Or get a specific object if you know its name (or id)
        tson::Layer *layer = map->getLayer("Main Layer");
        tson::Tileset *tileset = map->getTileset("demo-tileset");
        tson::Tile *tile = tileset->getTile(1); //Tileson tile-IDs starts with 1, to be consistent
        // with IDs in data lists. This is in other words the
        //first tile.

        //For tile layers, you can get the tiles presented as a 2D map by calling getTileData()
        //Using x and y positions in tile units.
        if(layer->getType() == tson::LayerType::TileLayer)
        {
            //When the map is of a fixed size, you can get the tiles like this
            if(!map->isInfinite())
            {
                std::map<std::tuple<int, int>, tson::Tile *> tileData = layer->getTileData();

                //Unsafe way to get a tile
                tson::Tile *invalidTile = tileData[{0, 4}]; // x:0,  y:4  - There is no tile here, so this will be nullptr.
                // Be careful with this, as it expands the map with an ID of {0,4} pointing
                // to a nullptr...

                //Individual tiles should be retrieved by calling the safe version:
                tson::Tile *safeInvalid = layer->getTileData(0, 5); //Another non-existent tile, but with safety check.
                //Will not expand the map with a nullptr

                tson::Tile *tile1 = layer->getTileData(4, 4);       //x:4,  y:4  - Points to tile with ID 1 (Tiled internal ID: 0)
                tson::Tile *tile2 = layer->getTileData(5, 4);       //x:5,  y:4  - Points to tile with ID 3 (Tiled internal ID: 2)
                tson::Tile *tile3 = layer->getTileData(8, 14);      //x:8,  y:14 - Points to tile with ID 2 (Tiled internal ID: 1)
                tson::Tile *tile4 = layer->getTileData(17, 5);      //x:17, y:5  - Points to tile with ID 5 (Tiled internal ID: 4)



                //You can of course also loop through every tile!
                for (const auto &[id, tile] : tileData)
                {
                    //Must check for nullptr, due to how we got the first invalid tile (pos: 0, 4)
                    //Would be unnecessary otherwise.
                    if(tile != nullptr)
                        int tileId = tile->getId(); //A bit verbose, as this is the same as id from the key, but you get the idea.
                }
            }
        }

        //If an object supports properties, you can easily get a property value by calling get<T>() or the property itself with getProp()
        int myInt = layer->get<int>("my_int");
        float myFloat = layer->get<float>("my_float");
        bool myBool = layer->get<bool>("my_bool");
        std::string myString = layer->get<std::string>("my_string");
        tson::Colori myColor = layer->get<tson::Colori>("my_color");
        #ifndef DISABLE_CPP17_FILESYSTEM
        fs::path file = layer->get<fs::path>("my_file");
        #else
        std::string file = layer->get<std::string>("my_file");
        #endif
        tson::Property *prop = layer->getProp("my_property");
    }
    else //Error occured
    {
        std::cout << map->getStatusMessage();
    }
    REQUIRE( true );
}

TEST_CASE( "A simple example on how to use data of objects and tiles (Single Header)", "[demo]" )
{
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);

    if(map->getStatus() == tson::ParseStatus::OK)
    {
        //Gets the layer called "Object Layer" from the "ultimate_demo.json map
        tson::Layer *objectLayer = map->getLayer("Object Layer"); //This is an Object Layer

        //Example from an Object Layer.
        if(objectLayer->getType() == tson::LayerType::ObjectGroup)
        {
            tson::Object *goomba = objectLayer->firstObj("goomba"); //Gets the first object with this name. This can be any object.

            //If you want to just go through every existing object in the layer:
            for(auto &obj : objectLayer->getObjects())
            {
                tson::Vector2i position = obj.getPosition();
                tson::Vector2i size = obj.getSize();
                tson::ObjectType objType = obj.getObjectType();

                //You may want to check the object type to make sure you use the data right.
            }

            tson::ObjectType objType = goomba->getObjectType();

            /*!
             * tson::ObjectType is defined like this.
             * They are automatically detected based on what kind of object you have created
             * enum class Type : uint8_t
                {
                    Undefined = 0,
                    Object = 1,
                    Ellipse = 2, //<-- Circle
                    Rectangle = 3,
                    Point = 4,
                    Polygon = 5,
                    Polyline = 6,
                    Text = 7,
                    Template = 8
                };
             */

            if (objType == tson::ObjectType::Rectangle)
            {
                tson::Vector2i size = goomba->getSize();
                tson::Vector2i position = goomba->getPosition();

                //If you have set a custom property, you can also get this
                int hp = goomba->get<int>("hp");

                //Using size and position you can can create a Rectangle object by your library of choice.
                //An example if you were about to use SFML for drawing:
                //sf::RectangleShape rect;
                //rect.setSize(sf::Vector2f(size.x, size.y));
                //rect.setPosition(sf::Vector2f(position.x, position.y));
            }
            else if (objType == tson::ObjectType::Polygon)
            {
                for(auto const &poly : goomba->getPolygons())
                {
                    //Set a point on a shape taking polygons
                }
                tson::Vector2i position = goomba->getPosition();
            }
            else if (objType == tson::ObjectType::Polyline)
            {
                std::vector<tson::Vector2i> polys = goomba->getPolylines();
                for(auto const &poly : goomba->getPolylines())
                {

                }
                tson::Vector2i position = goomba->getPosition();
            }
        }

        tson::Layer *tileLayer = map->getLayer("Main Layer"); //This is a Tile Layer.
        tson::Tileset *tileset = map->getTileset("demo-tileset"); //You will also need the tileset used
        //by the tile map to make sense of everything

        int firstId = tileset->getFirstgid(); //First tile id of the tileset
        int columns = tileset->getColumns(); //For the demo map it is 8.
        int rows = tileset->getTileCount() / columns;
        int lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

        //Example from a Tile Layer
        //I know for a fact that this is a Tile Layer, but you can check it this way to be sure.
        if(tileLayer->getType() == tson::LayerType::TileLayer)
        {
            //pos = position in tile units
            for(auto &[pos, tile] : tileLayer->getTileData()) //Loops through absolutely all existing tiles
            {
                #ifndef DISABLE_CPP17_FILESYSTEM
                fs::path imagePath;
                std::string pathStr;
                #else
                std::string imagePath;
                #endif
                //With this, I know that it's related to the tileset above (though I only have one tileset)
                if(tile->getId() >= firstId && tile->getId() <= lastId)
                {
                    #ifndef DISABLE_CPP17_FILESYSTEM
                    imagePath = tileset->getImagePath();
                    pathStr = imagePath.u8string();
                    #else
                    imagePath = tileset->getImagePath();
                    #endif
                }

                //Get position in pixel units
                tson::Vector2i position = {std::get<0>(pos) * map->getTileSize().x,std::get<1>(pos) * map->getTileSize().y};

                int baseTilePosition = (tile->getId() - firstId); //This will determine the base position of the tile.

                //The baseTilePosition can be used to calculate offset on its related tileset image.
                int tileModX = (baseTilePosition % columns);
                int currentRow = (baseTilePosition / columns);
                int offsetX = (tileModX != 0) ? ((tileModX) * map->getTileSize().x) : (0 * map->getTileSize().x);
                int offsetY =  (currentRow < rows-1) ? (currentRow * map->getTileSize().y) : ((rows-1) * map->getTileSize().y);

                //Here you can determine the offset that should be set on a sprite
                //Example on how it would be done using SFML (where sprite presumably is a member of a generated game object):
                //sprite.setTextureRect({offsetX, offsetY, map->getTileSize().x, map->getTileSize().y});
                //sprite.setPosition({position.x, position.y});
            }
        }
    }
    REQUIRE( true );
}

#endif
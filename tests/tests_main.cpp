//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/catch.hpp"
#include "../src/Tileson.h"

#include "tson_files_mapper.h"
#include "../TilesonConfig.h"

bool mapIsAbsolutelyFine(tson::Map &map)
{
    auto main = map.getLayer("Main Layer");
    auto tileData = main->getTileData();
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
            !map.getLayer("Object Layer")->getObjectsByType(tson::Object::Type::Object).empty() &&
            map.getLayer("Object Layer")->getObj(2)->getName() == "coin" &&
            map.getTileset("demo-tileset") != nullptr &&
            map.getTileset("demo-tileset")->getTile(36) != nullptr &&
            map.getTileset("demo-tileset")->getTile(36)->getAnimation().size() == 2 &&
            map.getTileset("demo-tileset")->getTerrain("test_terrain")->getProperties().getSize() == 2 &&
            map.getTileset("demo-tileset")->getTerrain("test_terrain")->getProp("i_like_this")->getType() == tson::Property::Type::Boolean &&
            !map.getTileset("demo-tileset")->getTerrain("test_terrain")->get<std::string>("description").empty() &&
            map.getTileMap().size() > 10 &&
            tileData[{4,4}] != nullptr && tileData[{4,4}]->getId() == 1 &&
            tileData[{5,4}] != nullptr && tileData[{5,4}]->getId() == 3 &&
            main->getTileData(8,14) != nullptr && main->getTileData(8,14)->getId() == 2 &&
            main->getTileData(17,5) != nullptr && main->getTileData(17,5)->getId() == 5
            );
}


TEST_CASE( "Parse a whole map by file", "[complete][parse][file]" ) {
    tson::Tileson t;
    #if USE_CPP17_FILESYSTEM
    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;
    #else
    std::string pathToUse = "../../content/test-maps/ultimate_test.json";
    #endif
    tson::Map map = t.parse({pathToUse});
    if(map.getStatus() == tson::Map::ParseStatus::OK)
        REQUIRE(mapIsAbsolutelyFine(map));
    else
    {
        std::cout << "Ignored - " << map.getStatusMessage() << std::endl;
        REQUIRE(true);
    }
}

TEST_CASE( "Parse a whole map by memory", "[complete][parse][memory]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);
    if (map.getStatus() == tson::Map::ParseStatus::OK)
        REQUIRE(mapIsAbsolutelyFine(map));
    else
    {
        std::cout << "Memory parse error - " << map.getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}

TEST_CASE( "Parse a simple map by memory - tiles without any properties (issue #4)", "[simple][parse][memory]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files_mapper::_SIMPLE_MAP_JSON, tson_files_mapper::_SIMPLE_MAP_JSON_SIZE);
    if (map.getStatus() == tson::Map::ParseStatus::OK)
    {
        auto main = map.getLayer("simple_layer");
        auto tileset = map.getTilesets()[0];
        auto tileData = main->getTileData();
        bool result = (tileData.size() > 16 && main->getTileData(0,0) != nullptr && tileset.getTiles().size() == 48);
        REQUIRE(result);
    }
    else
    {
        std::cout << "Memory parse error - " << map.getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}

TEST_CASE( "Parse a minimal version of whole map by memory", "[complete][parse][memory]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_MIN_JSON, tson_files::_ULTIMATE_TEST_MIN_JSON_SIZE);
    if (map.getStatus() == tson::Map::ParseStatus::OK)
        REQUIRE(mapIsAbsolutelyFine(map));
    else
    {
        std::cout << "Memory parse error - " << map.getStatusMessage() << std::endl;
        //REQUIRE(true);
        FAIL("Unexpected memory read failure!");
    }
}



TEST_CASE( "Go through demo code - get success", "[demo]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);

    if(map.getStatus() == tson::Map::ParseStatus::OK)
    {
        //Get color as an rgba color object
        tson::Colori bgColor = map.getBackgroundColor(); //RGBA with 0-255 on each channel

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
        for (auto &layer : map.getLayers())
        {
            if (layer.getType() == tson::Layer::Type::ObjectGroup)
            {
                for (auto &obj : layer.getObjects())
                {
                    //Just iterate through all the objects
                }
                //Or use these queries:

                //Gets the first object it find with the name specified
                tson::Object *player = layer.firstObj("player"); //Does not exist in demo map...

                //Gets all objects with a matching name
                std::vector<tson::Object> enemies = layer.getObjectsByName("goomba"); //But we got two of those

                //Gets all objects of a specific type
                std::vector<tson::Object> objects = layer.getObjectsByType(tson::Object::Type::Object);

                //Gets an unique object by its name.
                tson::Object *uniqueObj = layer.getObj(2);
            }
        }

        //Or get a specific object if you know its name (or id)
        tson::Layer *layer = map.getLayer("Main Layer");
        tson::Tileset *tileset = map.getTileset("demo-tileset");
        tson::Tile *tile = tileset->getTile(1); //Tileson tile-IDs starts with 1, to be consistent
        // with IDs in data lists. This is in other words the
        //first tile.

        //For tile layers, you can get the tiles presented as a 2D map by calling getTileData()
        //Using x and y positions in tile units.
        if(layer->getType() == tson::Layer::Type::TileLayer)
        {
            //When the map is of a fixed size, you can get the tiles like this
            if(!map.isInfinite())
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
        #if USE_CPP17_FILESYSTEM
        fs::path file = layer->get<fs::path>("my_file");
        #else
        std::string file = layer->get<std::string>("my_file");
        #endif
        tson::Property *prop = layer->getProp("my_property");
    }
    else //Error occured
    {
        std::cout << map.getStatusMessage();
    }
    REQUIRE( true );
}

TEST_CASE( "A simple example on how to use data of objects and tiles", "[demo]" )
{
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);

    if(map.getStatus() == tson::Map::ParseStatus::OK)
    {
        //Gets the layer called "Object Layer" from the "ultimate_demo.json map
        tson::Layer *objectLayer = map.getLayer("Object Layer"); //This is an Object Layer

        //Example from an Object Layer.
        if(objectLayer->getType() == tson::Layer::Type::ObjectGroup)
        {
            tson::Object *goomba = objectLayer->firstObj("goomba"); //Gets the first object with this name. This can be any object.

            //If you want to just go through every existing object in the layer:
            for(auto &obj : objectLayer->getObjects())
            {
                tson::Vector2i position = obj.getPosition();
                tson::Vector2i size = obj.getSize();
                tson::Object::Type objType = obj.getObjectType();

                //You may want to check the object type to make sure you use the data right.
            }

            tson::Object::Type objType = goomba->getObjectType();

            /*!
             * tson::Object::Type is defined like this.
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

            if (objType == tson::Object::Type::Rectangle)
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
            else if (objType == tson::Object::Type::Polygon)
            {
                for(auto const &poly : goomba->getPolygons())
                {
                    //Set a point on a shape taking polygons
                }
                tson::Vector2i position = goomba->getPosition();
            }
            else if (objType == tson::Object::Type::Polyline)
            {
                std::vector<tson::Vector2i> polys = goomba->getPolylines();
                for(auto const &poly : goomba->getPolylines())
                {

                }
                tson::Vector2i position = goomba->getPosition();
            }
        }

        tson::Layer *tileLayer = map.getLayer("Main Layer"); //This is a Tile Layer.
        tson::Tileset *tileset = map.getTileset("demo-tileset"); //You will also need the tileset used
                                                                       //by the tile map to make sense of everything

        int firstId = tileset->getFirstgid(); //First tile id of the tileset
        int columns = tileset->getColumns(); //For the demo map it is 8.
        int rows = tileset->getTileCount() / columns;
        int lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

        //Example from a Tile Layer
        //I know for a fact that this is a Tile Layer, but you can check it this way to be sure.
        if(tileLayer->getType() == tson::Layer::Type::TileLayer)
        {
            //pos = position in tile units
            for(auto &[pos, tile] : tileLayer->getTileData()) //Loops through absolutely all existing tiles
            {
                #if USE_CPP17_FILESYSTEM
                fs::path imagePath;
                std::string pathStr;
                #else
                std::string imagePath;
                #endif
                //With this, I know that it's related to the tileset above (though I only have one tileset)
                if(tile->getId() >= firstId && tile->getId() <= lastId)
                {
                    #if USE_CPP17_FILESYSTEM
                    imagePath = tileset->getImagePath();
                    pathStr = imagePath.u8string();
                    #else
                    imagePath = tileset->getImagePath();
                    #endif
                }

                //Get position in pixel units
                tson::Vector2i position = {std::get<0>(pos) * map.getTileSize().x,std::get<1>(pos) * map.getTileSize().y};

                int baseTilePosition = (tile->getId() - firstId); //This will determine the base position of the tile.

                //The baseTilePosition can be used to calculate offset on its related tileset image.
                int tileModX = (baseTilePosition % columns);
                int currentRow = (baseTilePosition / columns);
                int offsetX = (tileModX != 0) ? ((tileModX) * map.getTileSize().x) : (0 * map.getTileSize().x);
                int offsetY =  (currentRow < rows-1) ? (currentRow * map.getTileSize().y) : ((rows-1) * map.getTileSize().y);

                //Here you can determine the offset that should be set on a sprite
                //Example on how it would be done using SFML (where sprite presumably is a member of a generated game object):
                //sprite.setTextureRect({offsetX, offsetY, map.getTileSize().x, map.getTileSize().y});
                //sprite.setPosition({position.x, position.y});
            }
        }
    }
    REQUIRE( true );
}
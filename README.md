[![Build Status](https://travis-ci.org/SSBMTonberry/tileson.svg?branch=master)](https://travis-ci.org/SSBMTonberry/tileson)

# Tileson
Tileson is a modern and helpful cross-platform json-parser for C++, used for parsing Tiled maps.

Tileson utilizes modern C++ (C++17) to create a stable, safe and helpful, but fast, parser for Tiled maps.
Including classes and functions to make it easier to use the Tiled data in your game. 
Tileson supports Tiled maps up to version `1.2.4`, but will probably be able to parse
maps made with newer versions of Tiled just as well.

# What is Tiled?
Tiled is a general purpose map editor developed by `Thorbjørn Lindeijer`.
Tiled is perfect if you want to create any map for 2D games, and is very popular.
Many commercial games have used it as their goto map editor. 
A few popular games that have used Tiled: `Shovel Knight`, `Axiom Verge` and `ScubaDiver`.


![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/tiled_tileson_image.png "Tiled Screenshot") 

Tiled can be found here:
- [Homepage](https://www.mapeditor.org/)
- [GitHub](https://github.com/bjorn/tiled)

# How to parse
Parsing a Tiled json 
```c++
#include "Tileson.h"

//Tileson uses an alias fs for std::filesystem.
int main()
    tson::Tileson parser;
    tson::Map map = parser.parse(fs::path("./path/to/map.json"));

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

    return 0;
}
```

Another quick example to showcase how to get data that can be used to produce drawable objects:
```c++
tson::Tileson t;
tson::Map map = t.parse(fs::path("./path/to/map.json"));

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
            int offsetY =  (currentRow < rows-1) ? (currentRow * map.getTileSize().y) : 
                           ((rows-1) * map.getTileSize().y);

            //Here you can determine the offset that should be set on a sprite
            //Example on how it would be done using SFML (where sprite presumably is a member of a generated game object):
            //sprite.setTextureRect({offsetX, offsetY, map.getTileSize().x, map.getTileSize().y});
            //sprite.setPosition({position.x, position.y});
        }
    }
}
``` 

# Compiling
The program is cross-platform. However, it does not compile on all compilers.
`std::filesystem` is used as a default for Linux and Windows systems, as it has been 
supported for a while. Unfortunately, the default compiler, `Apple Clang`, shipped with todays Apple OSX (as of `24.08.2019`) does not support `std::filesystem` at all. For this reason, `std::filesystem` is disabled as default for `Apple` systems. This can however be activated by turning the cmake-flag `USE_CPP17_FILESYSTEM` on.

As a default the library is built statically, and `std::filesystem` is used (EXCEPT for Apple OSX), but this can be changed by changing the options in CMake. 

## Windows
While you probably can use other compilers as well, the only supported compiler for Tileson is the default shipped with Visual Studio: `MSVC`. The C++17 features used in this library has been available for quite a while, so you should be able to compile this easily. Just open the `CMakeLists.txt` file in the `CMake` gui, or simply use the `cmake CMakeLists.txt` command inside the project folder (if Visual Studio is your system default).

## Linux
If you have `GCC7`, `GCC8`, `GCC9` or newer as a compiler on your system, you should be good to go!
If you are okay with the default settings: Just calling the `cmake CMakeLists.txt` inside the project folder will create a `Makefile` for you. Then you can compile the library by simply calling `make`.

## Apple (OSX)
As `std::filesystem` is not supported by the version of `Apple Clang` shipped with `Mac OSX 10.14 (Mojave)`, this functionality is disabled by default when using this system. You can, however, install the newest version of `llvm` via `Homebrew`, which actually has supported `std::filesystem` for a while.
To generate a solution to be able to build this library, you will need to open the `CMakeLists.txt` file in `CMake`. If `CMake` is not installed on your system, it can easily be found on the internet.

# Examples
You can find examples on how to draw the demo map using frameworks like SFML under [examples](https://github.com/SSBMTonberry/tileson/tree/master/examples).
To build this, you will have to enable the CMake flag `BUILD_EXAMPLES`. All examples requires `C++17 std::filesystem` to work, and will fail to build on compilers not supporting this feature.
For Windows systems, you will need to include the [openal32.dll](https://github.com/SSBMTonberry/tileson/tree/master/external_libs/libs/win/release/msvc/sfml)-file 
in the executable directory to be able to run the demo.


# Libraries used by Tileson
- [Catch2](https://github.com/catchorg/Catch2/) - For Unit Tests
- [JSON for Modern C++](https://github.com/nlohmann/json) - For JSON read/write

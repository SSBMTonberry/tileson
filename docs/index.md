
# Tileson
Tileson is a modern and helpful cross-platform json-parser for C++, used for parsing Tiled maps.

Tileson utilizes modern C++ (C++17) to create a stable, safe and helpful, but fast, parser for Tiled maps.
Including classes and functions to make it easier to use the Tiled data in your game. 
Tileson supports Tiled maps up to version `1.4.1`, but will probably be able to parse
maps made with newer versions of Tiled just as well.

Be sure to take a look at the release notes to see what's new!

## Documentation

There is a `Doxygen` generated documentation of Tileson that can be found [HERE](https://ssbmtonberry.github.io/tileson/html)

# Tileson is header-only!
This means that all you need is one file, `single_include/tileson.hpp` to have Tileson going
in your project! The single-file is generated, and is quite large, as it uses the nlohmann/json as a built-in dependency,
and might thus be a bit heavy to load (26 000 lines of code). 

There is also an option to use a `tileson_min.hpp` file which only contains the `Tileson` related code (~5000 lines of code), but this requires you to include your own version of [nlohmann/json](https://github.com/nlohmann/json) before including `tileson_min.hpp`

You may alternatively copy the `include` directory and all its contents if you
want to have every component in their own file. This will probably be way less heavy for your IDE, but you will still only need to include the `tileson.h` file in the top level. 

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
#include "tileson.hpp"

//Tileson uses an alias fs for std::filesystem.
int main()
    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(fs::path("./path/to/map.json"));

    if(map->getStatus() == tson::ParseStatus::OK)
    {
        //Get color as an rgba color object
        tson::Colori bgColor = map->getBackgroundColor(); //RGBA with 0-255 on each channel

        //This color can be compared with Tiled hex string
        if (bgColor == "#ffaa07")
            printf("Cool!");

        //Or you can compare them with other colors
        tson::Colori otherColor{255, 170, 7, 255};
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

                //Gets the first object it finds with the name specified
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

                //New in v1.2.0
                //You can now get tiles with positions and drawing rect via tson::TileObject
                //Drawing rects are also accessible through tson::Tile.
                tson::TileObject *tileobj1 = layer->getTileObject(4, 4);
                tson::Vector2f position = tileobj1->getPosition();
                tson::Rect drawingRect = tileobj1->getDrawingRect();

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

    return 0;
}
```

### Another quick example to showcase how to get data that can be used to produce drawable objects:
```c++
tson::Tileson t;
std::unique_ptr<tson::Map> map = t.parse(fs::path("./path/to/map.json"));

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

    //You can get your tileset like this, but in v1.2.0
    //The tiles themselves holds a pointer to their related tileset.
    tson::Tileset *tileset = map->getTileset("demo-tileset");

    //Example from a Tile Layer
    //I know for a fact that this is a Tile Layer, but you can check it this way to be sure.
    if(tileLayer->getType() == tson::LayerType::TileLayer)
    {
        //pos = position in tile units
        for(auto &[pos, tileObject] : tileLayer->getTileObjects()) //Loops through absolutely all existing tileObjects
        {
            tson::Tileset *tileset = tileObject.getTile()->getTileset();
            tson::Rect drawingRect = tileObject.getDrawingRect();
            tson::Vector2f position = tileObject.getPosition();

            //Here you can determine the offset that should be set on a sprite
            //Example on how it would be done using SFML (where sprite presumably is a member of a generated game object):
            //sf::Sprite *sprite = storeAndLoadImage(tileset->getImage().u8string(), {0, 0});
            //if (sprite != nullptr)
            //{
            //    sprite->setTextureRect({drawingRect.x, drawingRect.y, drawingRect.width, drawingRect.height});
            //    sprite->setPosition({position.x, position.y});
            //    m_window.draw(*sprite);
            //}
        }
    }
}
``` 

### Parsing worlds

- Tileson now supports `Tiled worlds`. These contains a collection of several maps that can be tied together, but the files themselves must be parsed separately using `Tileson`. **NB! This functionality requires `std::filesystem` to be enabled (DISABLE_CPP17_FILESYSTEM not defined (default))**. (See `examples` to get a full idea how worlds works):

```c++
tson::Tileson t;
tson::World world; 
world.parse(fs::path("path/to/you/file.world"));

//world.get("w1.json") can be used to get a specific map

for(const auto &data : world.getMapData())
{
    std::unique_ptr<tson::Map> map = t.parse(fs::path(world.getFolder() / data.fileName));
    //...
}

```

### Parsing Tiled-projects

- Tileson now supports `Tiled projects`. These contains all `map` and `world` data, but the files themselves must be parsed separately using `Tileson`. **NB! This functionality requires `std::filesystem` to be enabled (DISABLE_CPP17_FILESYSTEM not defined (default))**. (See `examples` to get a full idea how projects works).

```c++
tson::Tileson t;
tson::Project project; 
bool ok = project.parse(fs::path("path/to/you/file.tiled-project"));

for(const auto &folder : m_project.getFolders())
{
    // You can check if a project folder contains a world with -> folder.hasWorldFile()
    // If it does, you can get the world data with -> folder.getWorld()
    for(const auto &file : folder.getFiles())
    {
        std::unique_ptr<tson::Map> map = t.parse(fs::path(folder.getPath() / file.filename()));
        //...
    }
}
```

# Compiling
The program is cross-platform. However, it does not compile on all compilers.
`std::filesystem` is used as a default for Linux and Windows systems, as it has been 
supported for a while. Unfortunately, the default compiler, `Apple Clang`, shipped with todays Apple OSX (as of `24.08.2019`) does not support `std::filesystem` at all. For this reason, `std::filesystem` is disabled as default for `Apple` systems. As of v1.1.0 filesystem is in use unless `DISABLE_CPP17_FILESYSTEM` is defined (`#define`). This can also be controlled as a CMake parameter. Also, the library itself is header-only, and will only need the headers to work (see top of README for details). 

As a default, compiling examples are disabled due to CI, but it can easily be enabled through the CMakeLists.txt file on the root level of the project. Also keep in mind that the examples are using content from the `tileson/content` folder and are using relative paths. This is where the executable usually is located when compiling, and must have the same relative path to find the files:

![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-example-destination.png "Demo path") 

## Windows
For compiling the demo on Windows, it's recommended to use either Visual Studio or Visual Studio Code and open the CMakeLists.txt on the root level of the project. Examples for Windows required a `x86`-build in the past, but `x64` should now work just as well! Some screenshots to showcase how CMake should be configured to build the demo using Visual Studio 2019:

#### Locate the CMakeLists.txt in the root of this project

![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile_2.png "VS 1") 

#### Configure build to be x86 or x64

![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile.png "VS 2") 

#### And then

![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile_3.png "VS 3")
![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile_4.png "VS 4")
![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile_5.png "VS 5")

#### Output after full build

![alt text](https://github.com/SSBMTonberry/tileson/blob/master/content/demo-vs-compile_6.png "VS 6")

## Linux

If you have `GCC7`, `GCC8`, `GCC9`, `GCC10` or newer as a compiler on your system, you should be good to go!
If you are okay with the default settings: Just calling the `cmake CMakeLists.txt` inside the project folder will create a `Makefile` for you. Then you can compile the library by simply calling `make`.

## Apple (OSX)

`std::filesystem` should be supported by the `Apple Clang`-compiler shipped with the latest version of `Mac OSX`, but may not work with older versions of this OS. If you are using an old version of OSX you can, however, install the newest version of `llvm` via `Homebrew`, which has supported `std::filesystem` for a while.
To generate a solution to be able to build this library, you will need to open the `CMakeLists.txt` file in `CMake`. If `CMake` is not installed on your system, it can easily be found on the internet.

# Examples

You can find examples on how to draw the `demo maps`/`projects` using frameworks like SFML under [examples](https://github.com/SSBMTonberry/tileson/tree/master/examples).
To build this, you will have to enable the CMake flag `BUILD_EXAMPLES`. All examples requires `C++17 std::filesystem` to work, and will fail to build on compilers not supporting this feature.

# Generating the single-header

If Linux is used, a single-header version of Tileson can be generated by calling the 
`amalgamate_script.sh` in the `tools/`folder.
Simply call it like this: `sh amalgamate_script.sh`. There is also a .bat-version of this script for Windows.


# Libraries used by Tileson

- [Catch2](https://github.com/catchorg/Catch2/) - For Unit Tests
- [JSON for Modern C++](https://github.com/nlohmann/json) - For JSON read/write
- [Amalgamate](https://github.com/SSBMTonberry/Amalgamate) - Fork of [vinniefalco/Amalgamate](https://github.com/vinniefalco/Amalgamate) - For generating single-header include of `tileson.hpp`

# Libraries used for examples/demo

- [SFML](https://github.com/SFML/SFML) - For drawing maps.
- [Dear ImGui](https://github.com/ocornut/imgui) - For displaying information and managing maps.
- [ImGui-SFML](https://github.com/eliasdaler/imgui-sfml) - To render Dear ImGui with SFML.

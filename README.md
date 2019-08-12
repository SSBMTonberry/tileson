# Tileson
A modern and helpful json-parser for Tiled maps.

Tileson utilizes modern C++ (C++17) to create a stable, safe and helpful, but fast, parser for Tiled maps.
Including classes and functions to make it easier to use the Tiled data in your game. 
Tileson supports Tiled maps up to version `1.2.4`, but will probably be able to parse
maps made with newer versions of Tiled just as well.

## How to parse
Parsing a Tiled json 
```c++
#include "Tileson.h"

//Tileson uses an alias fs for std::filesystem.
int main()
    tson::Tileson parser;
    tson::Map map = parser.parse(fs::path("./path/to/map.json"));

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

    //Get a specific object if you know its name (or id)
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

    return 0;
}
```
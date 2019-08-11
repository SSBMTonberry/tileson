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
    for(auto &item : map.getLayers())
    {
        //Do something
    }

    //Or get a specific object if you know its name (or id)
    tson::Layer *layer = map.getLayer("Main Layer");

    //If an object supports properties, you can easily get a property value by calling get<T>() or the property itself with getProp()
    int myInt = layer->get<int>("my_int");
    Property *prop = layer->getProp("my_property");

    return 0;
}
```
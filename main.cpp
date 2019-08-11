#include "src/Tileson.h"

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
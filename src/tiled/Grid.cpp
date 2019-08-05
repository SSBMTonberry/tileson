//
// Created by robin on 05.08.2019.
//

#include "Grid.h"

tson::Grid::Grid(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Grid::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("orientation") > 0) m_orientation = json["orientation"].get<std::string>(); //Optional

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;

    return allFound;
}

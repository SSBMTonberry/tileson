//
// Created by robin on 03.08.2019.
//

#include "WangColor.h"

tson::WangColor::WangColor(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangColor::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("color") > 0) m_color = tson::Color(json["color"].get<std::string>()); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("probability") > 0) m_probability = json["probability"].get<float>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

    return allFound;
}

const tson::Color &tson::WangColor::getColor() const
{
    return m_color;
}

const std::string &tson::WangColor::getName() const
{
    return m_name;
}

float tson::WangColor::getProbability() const
{
    return m_probability;
}

int tson::WangColor::getTile() const
{
    return m_tile;
}

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

    if(json.count("color") > 0) m_color = tson::Colori(json["color"].get<std::string>()); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("probability") > 0) m_probability = json["probability"].get<float>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

    return allFound;
}

/*!
 * 'color': Color object created from hex-formatted string (#RRGGBB or #AARRGGBB)
 * @return
 */
const tson::Colori &tson::WangColor::getColor() const
{
    return m_color;
}

/*!
 * 'name': Name of the Wang color
 * @return
 */
const std::string &tson::WangColor::getName() const
{
    return m_name;
}

/*!
 * 'probability': Probability used when randomizing
 * @return
 */
float tson::WangColor::getProbability() const
{
    return m_probability;
}

/*!
 * 'tile': Local ID of tile representing the Wang color
 * @return
 */
int tson::WangColor::getTile() const
{
    return m_tile;
}

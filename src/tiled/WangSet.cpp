//
// Created by robin on 03.08.2019.
//

#include "WangSet.h"

tson::WangSet::WangSet(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangSet::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;

    //More advanced data
    if(json.count("wangtiles") > 0 && json["wangtiles"].is_array())
        std::for_each(json["wangtiles"].begin(), json["wangtiles"].end(), [&](const nlohmann::json &item) { m_wangTiles.emplace_back(item); });
    if(json.count("cornercolors") > 0 && json["cornercolors"].is_array())
        std::for_each(json["cornercolors"].begin(), json["cornercolors"].end(), [&](const nlohmann::json &item) { m_cornerColors.emplace_back(item); });
    if(json.count("edgecolors") > 0 && json["edgecolors"].is_array())
        std::for_each(json["edgecolors"].begin(), json["edgecolors"].end(), [&](const nlohmann::json &item) { m_edgeColors.emplace_back(item); });
    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

/*!
 * 'name': Name of the Wang set
 * @return
 */
const std::string &tson::WangSet::getName() const
{
    return m_name;
}

/*!
 * 'tile': Local ID of tile representing the Wang set
 * @return
 */
int tson::WangSet::getTile() const
{
    return m_tile;
}

/*!
 * 'wangtiles': Array of Wang tiles
 * @return
 */
const std::vector<tson::WangTile> &tson::WangSet::getWangTiles() const
{
    return m_wangTiles;
}

/*!
 * 'cornercolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getCornerColors() const
{
    return m_cornerColors;
}

/*!
 * 'edgecolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getEdgeColors() const
{
    return m_edgeColors;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::WangSet::getProperties()
{
    return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::WangSet::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);

    return nullptr;
}

//
// Created by robin on 03.08.2019.
//

#include "Terrain.h"

tson::Terrain::Terrain(std::string name, int tile) : m_name {std::move(name)}, m_tile {tile}
{

}

tson::Terrain::Terrain(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Terrain::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

/*!
 * 'name': Name of terrain
 * @return
 */
const std::string &tson::Terrain::getName() const
{
    return m_name;
}

/*!
 * 'tile': Local ID of tile representing terrain
 * @return
 */
int tson::Terrain::getTile() const
{
    return m_tile;
}

/*!
 * 'properties': A list of properties (name, value, type). *Missing from the official Tiled documentation...*
 * @return
 */
tson::PropertyCollection &tson::Terrain::getProperties()
{
    return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Terrain::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

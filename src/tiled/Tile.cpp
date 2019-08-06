//
// Created by robin on 03.08.2019.
//

#include "Tile.h"


tson::Tile::Tile(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Tile::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
    if(json.count("id") > 0) m_id = json["id"].get<int>(); else allFound = false;
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional
    if(json.count("objectgroup") > 0) m_objectgroup = tson::Layer(json["objectgroup"]); //Optional

    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional

    //More advanced data
    if(json.count("animation") > 0 && json["animation"].is_array())
        std::for_each(json["animation"].begin(), json["animation"].end(), [&](const nlohmann::json &item) { m_animation.emplace_back(item); });
    if(json.count("terrain") > 0 && json["terrain"].is_array())
        std::for_each(json["terrain"].begin(), json["terrain"].end(), [&](const nlohmann::json &item) { m_terrain.emplace_back(item.get<int>()); });

    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

int tson::Tile::getId() const
{
    return m_id;
}

const fs::path &tson::Tile::getImage() const
{
    return m_image;
}

const tson::Vector2i &tson::Tile::getImageSize() const
{
    return m_imageSize;
}

const std::string &tson::Tile::getType() const
{
    return m_type;
}

const std::vector<tson::Frame> &tson::Tile::getAnimation() const
{
    return m_animation;
}

const tson::Layer &tson::Tile::getObjectgroup() const
{
    return m_objectgroup;
}

tson::PropertyCollection &tson::Tile::getProperties()
{
    return m_properties;
}

const std::vector<int> &tson::Tile::getTerrain() const
{
    return m_terrain;
}

//
// Created by robin on 02.08.2019.
//

#include "Tileset.h"

tson::Tileset::Tileset(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Tileset::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("columns") > 0) m_columns = json["columns"].get<int>(); else allFound = false;
    if(json.count("firstgid") > 0) m_firstgid = json["firstgid"].get<int>(); else allFound = false;
    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); else allFound = false;
    if(json.count("margin") > 0) m_margin = json["margin"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("spacing") > 0) m_spacing = json["spacing"].get<int>(); else allFound = false;
    if(json.count("tilecount") > 0) m_tileCount = json["tilecount"].get<int>(); else allFound = false;
    if(json.count("transparentcolor") > 0) m_transparentColor = tson::Color(json["transparentcolor"].get<std::string>()); //Optional
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); 
    if(json.count("grid") > 0) m_grid = tson::Grid(json["grid"]);

    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; else allFound = false;
    if(json.count("tilewidth") > 0 && json.count("tileheight") > 0)
        m_tileSize = {json["tilewidth"].get<int>(), json["tileheight"].get<int>()}; else allFound = false;
    if(json.count("tileoffset") > 0)
        m_tileOffset = {json["tileoffset"]["x"].get<int>(), json["tileoffset"]["y"].get<int>()};

    //More advanced data
    if(json.count("wangsets") > 0 && json["wangsets"].is_array())
        std::for_each(json["wangsets"].begin(), json["wangsets"].end(), [&](const nlohmann::json &item) { m_wangsets.emplace_back(item); });
    if(json.count("tiles") > 0 && json["tiles"].is_array())
        std::for_each(json["tiles"].begin(), json["tiles"].end(), [&](const nlohmann::json &item) { m_tiles.emplace_back(item); });
    if(json.count("terrains") > 0 && json["terrains"].is_array())
        std::for_each(json["terrains"].begin(), json["terrains"].end(), [&](const nlohmann::json &item) { m_terrains.emplace_back(item); });

    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

int tson::Tileset::getColumns() const
{
    return m_columns;
}

int tson::Tileset::getFirstgid() const
{
    return m_firstgid;
}

const fs::path &tson::Tileset::getImagePath() const
{
    return m_image;
}

const tson::Vector2i &tson::Tileset::getImageSize() const
{
    return m_imageSize;
}

int tson::Tileset::getMargin() const
{
    return m_margin;
}

const std::string &tson::Tileset::getName() const
{
    return m_name;
}

int tson::Tileset::getSpacing() const
{
    return m_spacing;
}

int tson::Tileset::getTileCount() const
{
    return m_tileCount;
}

const tson::Vector2i &tson::Tileset::getTileSize() const
{
    return m_tileSize;
}

const tson::Color &tson::Tileset::getTransparentColor() const
{
    return m_transparentColor;
}

const std::string &tson::Tileset::getType() const
{
    return m_type;
}

const fs::path &tson::Tileset::getImage() const
{
    return m_image;
}

const std::vector<tson::Tile> &tson::Tileset::getTiles() const
{
    return m_tiles;
}

const std::vector<tson::WangSet> &tson::Tileset::getWangsets() const
{
    return m_wangsets;
}

tson::PropertyCollection &tson::Tileset::getProperties()
{
    return m_properties;
}

const std::vector<tson::Terrain> &tson::Tileset::getTerrains() const
{
    return m_terrains;
}

const tson::Vector2i &tson::Tileset::getTileOffset() const
{
    return m_tileOffset;
}

const tson::Grid &tson::Tileset::getGrid() const
{
    return m_grid;
}

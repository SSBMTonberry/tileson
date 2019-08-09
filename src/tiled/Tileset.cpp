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
    if(json.count("transparentcolor") > 0) m_transparentColor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
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

/*!
 * 'columns': The number of tile columns in the tileset
 * @return
 */
int tson::Tileset::getColumns() const
{
    return m_columns;
}

/*!
 * 'firstgid': GID corresponding to the first tile in the set
 * @return
 */
int tson::Tileset::getFirstgid() const
{
    return m_firstgid;
}

/*!
 * 'image': Image used for tiles in this set
 * @return
 */
const fs::path &tson::Tileset::getImagePath() const
{
    return m_image;
}

/*!
 * x = 'imagewidth' and y = 'imageheight': in pixels
 * @return
 */
const tson::Vector2i &tson::Tileset::getImageSize() const
{
    return m_imageSize;
}

/*!
 * 'margin': Buffer between image edge and first tile (pixels)
 * @return
 */
int tson::Tileset::getMargin() const
{
    return m_margin;
}

/*!
 * 'name': Name given to this tileset
 * @return
 */
const std::string &tson::Tileset::getName() const
{
    return m_name;
}

/*!
 * 'spacing': Spacing between adjacent tiles in image (pixels)
 * @return
 */
int tson::Tileset::getSpacing() const
{
    return m_spacing;
}

/*!
 * 'tilecount': The number of tiles in this tileset
 * @return
 */
int tson::Tileset::getTileCount() const
{
    return m_tileCount;
}

/*!
 * x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set
 * @return
 */
const tson::Vector2i &tson::Tileset::getTileSize() const
{
    return m_tileSize;
}

/*!
 * 'transparentcolor': Color object created by hex-formatted color (#RRGGBB) (optional)
 * @return
 */
const tson::Colori &tson::Tileset::getTransparentColor() const
{
    return m_transparentColor;
}

/*!
 * 'type': tileset (for tileset files, since 1.0)
 * @return
 */
const std::string &tson::Tileset::getType() const
{
    return m_type;
}

/*!
 * 'image': Image used for tiles in this set
 * @return
 */
const fs::path &tson::Tileset::getImage() const
{
    return m_image;
}

/*!
 * 'tiles': Array of Tiles (optional)
 * @return
 */
const std::vector<tson::Tile> &tson::Tileset::getTiles() const
{
    return m_tiles;
}

/*!
 * 'wangsets':Array of Wang sets (since Tiled 1.1.5)
 * @return
 */
const std::vector<tson::WangSet> &tson::Tileset::getWangsets() const
{
    return m_wangsets;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Tileset::getProperties()
{
    return m_properties;
}

/*!
 * 'terrains': Array of Terrains (optional)
 * @return
 */
const std::vector<tson::Terrain> &tson::Tileset::getTerrains() const
{
    return m_terrains;
}

/*!
 * 'x' and 'y': See <tileoffset> (optional)
 * @return
 */
const tson::Vector2i &tson::Tileset::getTileOffset() const
{
    return m_tileOffset;
}

/*!
 * 'grid': This element is only used in case of isometric orientation, and determines
 * how tile overlays for terrain and collision information are rendered.
 * @return
 */
const tson::Grid &tson::Tileset::getGrid() const
{
    return m_grid;
}

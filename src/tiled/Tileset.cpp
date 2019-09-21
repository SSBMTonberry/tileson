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
    #if USE_CPP17_FILESYSTEM
    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); else allFound = false;
    #else
    if(json.count("image") > 0) m_image = json["image"].get<std::string>(); else allFound = false;
    #endif
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

    generateMissingTiles();

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
#if USE_CPP17_FILESYSTEM
const fs::path &tson::Tileset::getImagePath() const { return m_image; }
#else
const std::string &tson::Tileset::getImagePath() const { return m_image; }
#endif
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
#if USE_CPP17_FILESYSTEM
const fs::path &tson::Tileset::getImage() const { return m_image; }
#else
const std::string &tson::Tileset::getImage() const { return m_image; }
#endif
/*!
 * 'tiles': Array of Tiles (optional)
 * @return
 */
std::vector<tson::Tile> &tson::Tileset::getTiles()
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

/*!
 * Gets a tile by ID (Tiled ID + 1)
 * @param id The ID of the tile stored in Tiled map + 1. Example: If ID was stored in Tiled map as 0, the corresponding value in Tileson is 1.
 * This is to make sure the IDs of tiles matches their references in containers.
 * @return A pointer to the Tile if found. nullptr otherwise.
 */
tson::Tile *tson::Tileset::getTile(int id)
{
    auto result = std::find_if(m_tiles.begin(), m_tiles.end(), [&](const tson::Tile & item) { return item.getId() == id;});
    if(result == m_tiles.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Get an existing Terrain object by name
 * @param name
 * @return A pointer to the Terrain if found. nullptr otherwise.
 */
tson::Terrain *tson::Tileset::getTerrain(const std::string &name)
{
    auto result = std::find_if(m_terrains.begin(), m_terrains.end(), [&](const tson::Terrain & item) { return item.getName() == name;});
    if(result == m_terrains.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Tileset::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);

    return nullptr;
}

/*!
 * Tiled only has tiles with a property stored in the map. This function makes sure even the ones with no properties will exist.
 */
void tson::Tileset::generateMissingTiles()
{
    std::vector<int> tileIds;
    for(auto &tile : m_tiles)
        tileIds.push_back(tile.getId());

    for(int i = m_firstgid; i < m_firstgid + m_tileCount; ++i)
    {
        if(std::count(tileIds.begin(), tileIds.end(), i) == 0)
        {
            m_tiles.emplace_back(Tile(i));
        }
    }
}

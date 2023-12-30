//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESET_HPP
#define TILESON_TILESET_HPP

//#include "../external/json.hpp"

#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "../objects/PropertyCollection.hpp"
#include "Transformations.hpp"
#include "WangSet.hpp"
#include "Tile.hpp"
#include "Terrain.hpp"
#include "Grid.hpp"
#include <functional>

namespace tson
{
    class Map;
    class Project;
    class Tileset
    {
        public:
            inline Tileset() = default;
            inline explicit Tileset(IJson &json, tson::Map *map);
            inline bool parse(IJson &json, tson::Map *map);

            [[nodiscard]] inline int getColumns() const;
            [[nodiscard]] inline int getFirstgid() const;

            [[nodiscard]] inline const fs::path &getImagePath() const;
            [[nodiscard]] inline const fs::path &getFullImagePath() const;
            [[nodiscard]] inline const fs::path &getImage() const;
            [[nodiscard]] inline const Vector2i &getImageSize() const;
            [[nodiscard]] inline int getMargin() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline int getSpacing() const;
            [[nodiscard]] inline int getTileCount() const;
            [[nodiscard]] inline const Vector2i &getTileSize() const;
            [[nodiscard]] inline const Colori &getTransparentColor() const;
            [[nodiscard]] inline const std::string& getTypeStr() const;
            [[nodiscard]] inline TilesetType getType() const;
            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */
            [[nodiscard]] inline std::vector<tson::Tile> &getTiles();
            [[nodiscard]] inline const std::vector<tson::WangSet> &getWangsets() const;
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline const std::vector<tson::Terrain> &getTerrains() const;
            [[nodiscard]] inline const Vector2i &getTileOffset() const;
            [[nodiscard]] inline const Grid &getGrid() const;
            [[nodiscard]] inline TileRenderSize getTileRenderSize() const;
            [[nodiscard]] inline FillMode getFillMode() const;

            inline tson::Tile * getTile(uint32_t id);
            inline tson::Terrain * getTerrain(const std::string &name);

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0-stuff
            [[nodiscard]] inline tson::Map *getMap() const;
            [[nodiscard]] inline ObjectAlignment getObjectAlignment() const;

            inline static tson::ObjectAlignment StringToAlignment(std::string_view str);

            //v1.3.0
            inline tson::Vector2i getMarginSpacingOffset(const tson::Vector2i &posInTileUnits);
            inline tson::WangSet * getWangset(const std::string &name);
            inline const Transformations &getTransformations() const;


            #ifndef TSON_TEST_ENABLED
        private:
            #endif

            [[nodiscard]] inline tson::Project *getProject() const;

            inline void generateMissingTiles();

            int                           m_columns {};       /*! 'columns': The number of tile columns in the tileset */
            int                           m_firstgid {};      /*! 'firstgid': GID corresponding to the first tile in the set */

            fs::path                      m_image;            /*! 'image': Image used for tiles in this set */
            fs::path                      m_imagePath;        /*!  Image path relative to the tileset json of this tileset */

            tson::Vector2i                m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            int                           m_margin {};        /*! 'margin': Buffer between image edge and first tile (pixels)*/
            std::string                   m_name;             /*! 'name': Name given to this tileset */
            int                           m_spacing {};       /*! 'spacing': Spacing between adjacent tiles in image (pixels)*/
            int                           m_tileCount {};     /*! 'tilecount': The number of tiles in this tileset */
            tson::Vector2i                m_tileSize;         /*! x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set */
            tson::Colori                  m_transparentColor; /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional) */
            std::string                   m_typeStr;          /*! 'type': tileset (for tileset files, since 1.0) */
            TilesetType                   m_type{ TilesetType::Undefined }; /*! Tileset type as enum */

            std::vector<tson::Tile>       m_tiles;            /*! 'tiles': Array of Tiles (optional) */
            std::vector<tson::WangSet>    m_wangsets;         /*! 'wangsets':Array of Wang sets (since 1.1.5) */
            tson::PropertyCollection      m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

            std::vector<tson::Terrain>    m_terrains;         /*! 'terrains': Array of Terrains (optional) */
            tson::Vector2i                m_tileOffset;       /*! 'x' and 'y': See <tileoffset> (optional) */
            tson::Grid                    m_grid;             /*! 'grid': This element is only used in case of isometric orientation, and determines
                                                                   how tile overlays for terrain and collision information are rendered. */

            //v1.2.0-stuff
            tson::ObjectAlignment         m_objectAlignment{tson::ObjectAlignment::Unspecified};  /*! 'objectalignment': Alignment to use for tile objects. Tiled 1.4.*/
            tson::Map *                   m_map;              /*! The map who owns this tileset */

            //v1.3.0-stuff
            fs::path                      m_source {};           /*! 'source': exists only when tileset is contained in an external file*/
            fs::path                      m_path {};             /*! Has the full path to the tileset if 'source' has an existing value */
            Transformations               m_transformations {};  /*! New in Tiled v1.5 - This element is used to describe which transformations can be applied to
                                                                     the tiles (e.g. to extend a Wang set by transforming existing tiles).*/

            //v1.4.0-stuff
            TileRenderSize                m_tileRenderSize {};   /*! 'tilerendersize': The size to use when rendering tiles from this tileset on a tile layer. Valid values are 'tile' (the default) and 'grid'.
 *                                                                    When set to 'grid', the tile is drawn at the tile grid size of the map. (since 1.9)*/
            FillMode                      m_fillMode {};         /*! 'fillmode': The fill mode to use when rendering tiles from this tileset. Valid values are 'stretch' (the default) and 'preserve-aspect-fit'.
 *                                                                    Only relevant when the tiles are not rendered at their native size, so this applies to resized tile objects or in combination with 'tilerendersize' set to 'grid'. (since 1.9)*/

            std::string                   m_classType {};              /*! 'class': The class of this map (since 1.9, defaults to “”). */
            std::shared_ptr<tson::TiledClass> m_class {};
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Tileset::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

tson::Tileset::Tileset(IJson &json, tson::Map *map)
{
    parse(json, map);
}

bool tson::Tileset::parse(IJson &json, tson::Map *map)
{
    m_map = map;
    bool allFound = true;

    if(json.count("firstgid") > 0) m_firstgid = json["firstgid"].get<int>(); else allFound = false;

    //Tileset is stored in external file if 'source' exists
    if(json.count("source") > 0)
    {
        if(!allFound)
            return allFound;

        std::string sourceStr = json["source"].get<std::string>();
        m_source = fs::path(sourceStr);
        m_path = json.directory() / m_source;

        if(!json.parse(m_path))
            return false;
    }


    if (json.count("columns") > 0)
    {
        m_columns = json["columns"].get<int>();
        if (m_columns == 0)
        {
            m_type = TilesetType::ImageCollectionTileset;
        }
        else if (m_columns > 0)
        {
            m_type = TilesetType::ImageTileset;
        }
    }
    else
    {
        allFound = false;
    }

    // Image tileset specific properties
    if (m_type == TilesetType::ImageTileset)
    {
        if (json.count("image") > 0)
        {
            m_image = fs::path(json["image"].get<std::string>());
            m_imagePath = m_path.parent_path() / m_image;
        }
        else
        {
            allFound = false;
        }

        if (json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        {
            m_imageSize = { json["imagewidth"].get<int>(), json["imageheight"].get<int>() };
        }
        else
        {
            allFound = false;
        }
    }

    if(json.count("margin") > 0) m_margin = json["margin"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("spacing") > 0) m_spacing = json["spacing"].get<int>(); else allFound = false;
    if(json.count("tilecount") > 0) m_tileCount = json["tilecount"].get<int>(); else allFound = false;
    if(json.count("transparentcolor") > 0) m_transparentColor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
    if (json.count("type") > 0) m_typeStr = json["type"].get<std::string>();
    if(json.count("grid") > 0) m_grid = tson::Grid(json["grid"]);
    if(json.count("class") > 0) m_classType = json["class"].get<std::string>();                     //Optional

    if(json.count("tilewidth") > 0 && json.count("tileheight") > 0)
        m_tileSize = {json["tilewidth"].get<int>(), json["tileheight"].get<int>()}; else allFound = false;
    if(json.count("tileoffset") > 0)
        m_tileOffset = {json["tileoffset"]["x"].get<int>(), json["tileoffset"]["y"].get<int>()};

    if(json.count("tilerendersize") > 0)
    {
        std::string tileRenderStr = json["tilerendersize"].get<std::string>();
        if(tileRenderStr == "tile") m_tileRenderSize = TileRenderSize::Tile;
        else if(tileRenderStr == "grid") m_tileRenderSize = TileRenderSize::Grid;
    }

    if(json.count("fillmode") > 0)
    {
        std::string fillmode = json["fillmode"].get<std::string>();
        if(fillmode == "stretch") m_fillMode = FillMode::Stretch;
        else if(fillmode == "preserve-aspect-fit") m_fillMode = FillMode::PreserveAspectFit;
    }

    //More advanced data
    if(json.count("wangsets") > 0 && json["wangsets"].isArray())
    {
        auto &wangsets = json.array("wangsets");
        std::for_each(wangsets.begin(), wangsets.end(), [&](std::unique_ptr<IJson> &item) { m_wangsets.emplace_back(*item, m_map); });
    }
    if(json.count("tiles") > 0 && json["tiles"].isArray())
    {
        auto &tiles = json.array("tiles");
        std::for_each(tiles.begin(), tiles.end(), [&](std::unique_ptr<IJson> &item) { m_tiles.emplace_back(*item, this, m_map); });
    }
    if(json.count("terrains") > 0 && json["terrains"].isArray())
    {
        auto &terrains = json.array("terrains");
        std::for_each(terrains.begin(), terrains.end(), [&](std::unique_ptr<IJson> &item) { m_terrains.emplace_back(*item); });
    }

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item, getProject()); });
    }

    if(json.count("objectalignment") > 0)
    {
        std::string alignment = json["objectalignment"].get<std::string>();
        m_objectAlignment = StringToAlignment(alignment);
    }

    if(json.count("transformations") > 0)
    {
        m_transformations.parse(json["transformations"]);
    }

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
const fs::path &tson::Tileset::getImagePath() const { return m_image; }

/*!
 * Gets a path to the image used in this tileset relative to the JSON
 * file defining this tileset.
 * @return A path that can be used to open the image file of this tileset.
 */
const fs::path &tson::Tileset::getFullImagePath() const { return m_imagePath; }

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
const std::string& tson::Tileset::getTypeStr() const
{
    return m_typeStr;
}

/*!
 * 'type': Tileset type as enum
 * @return
*/
tson::TilesetType tson::Tileset::getType() const
{
    return m_type;
}

/*!
 * 'image': Image used for tiles in this set
 * @return
 */

const fs::path &tson::Tileset::getImage() const { return m_image; }

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
tson::Tile *tson::Tileset::getTile(uint32_t id)
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
    std::vector<uint32_t> tileIds;
    for(auto &tile : m_tiles)
        tileIds.push_back(tile.getId());

    for (uint32_t i = 1; i < (uint32_t)m_tileCount + 1; ++i)
    {
        if (std::count(tileIds.begin(), tileIds.end(), i) == 0)
        {
            m_tiles.emplace_back(Tile::CreateMissingTile(i, this, m_map));
        }
    }
}

/*!
 * Used for getting the tson::Map who is the parent of this Tileset.
 * @return a pointer to the tson::Map where this tileset is contained.
 */
tson::Map *tson::Tileset::getMap() const
{
    return m_map;
}

/*!
 *
 * @param str The string you want to convert
 * @return Alignment enum based on the string from the input.
 */
tson::ObjectAlignment tson::Tileset::StringToAlignment(std::string_view str)
{
    if(str == "unspecified") return tson::ObjectAlignment::Unspecified;
    else if(str == "topleft") return tson::ObjectAlignment::TopLeft;
    else if(str == "top") return tson::ObjectAlignment::Top;
    else if(str == "topright") return tson::ObjectAlignment::TopRight;
    else if(str == "left") return tson::ObjectAlignment::Left;
    else if(str == "center") return tson::ObjectAlignment::Center;
    else if(str == "right") return tson::ObjectAlignment::Right;
    else if(str == "bottomleft") return tson::ObjectAlignment::BottomLeft;
    else if(str == "bottom") return tson::ObjectAlignment::Bottom;
    else if(str == "bottomright") return tson::ObjectAlignment::BottomRight;
    else
        return tson::ObjectAlignment::Unspecified;
}

tson::ObjectAlignment tson::Tileset::getObjectAlignment() const
{
    return m_objectAlignment;
}

/*!
 * Helper function to calculate the correct additional offset when margin and/or spacing is used in a tileset
 * Created to solve issue #31.
 *
 * @param posInTileUnits Position of the current tile in tile units.
 * @return Calculated additional offset in pixels.
 */
tson::Vector2i tson::Tileset::getMarginSpacingOffset(const tson::Vector2i &posInTileUnits)
{
    if(m_margin == 0 && m_spacing == 0)
        return {0,0};

    tson::Vector2i offset {(posInTileUnits.x * m_spacing) + m_margin, (posInTileUnits.y * m_spacing) + m_margin};
    return offset;
}

/*!
 * Get a wangset by name
 * @param name
 * @return
 */
tson::WangSet *tson::Tileset::getWangset(const std::string &name)
{
    auto wangset = std::find_if(m_wangsets.begin(), m_wangsets.end(), [&](const auto &w) { return w.getName() == name; });

    if(wangset != m_wangsets.end())
        return &wangset.operator*();

    return nullptr;
}

/*!
 * New in Tiled v1.5 - This element is used to describe which transformations can be applied to
 * the tiles (e.g. to extend a Wang set by transforming existing tiles).
 *
 * @return
 */
const tson::Transformations &tson::Tileset::getTransformations() const
{
    return m_transformations;
}

tson::TileRenderSize tson::Tileset::getTileRenderSize() const
{
    return m_tileRenderSize;
}

tson::FillMode tson::Tileset::getFillMode() const
{
    return m_fillMode;
}

const std::string &tson::Tileset::getClassType() const
{
    return m_classType;
}


#endif //TILESON_TILESET_HPP
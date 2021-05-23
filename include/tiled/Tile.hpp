//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILE_HPP
#define TILESON_TILE_HPP

//#include "../external/json.hpp"
#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "Frame.hpp"
#include "Animation.hpp"
#include "../objects/PropertyCollection.hpp"
#include "Layer.hpp"
#include "../objects/Rect.hpp"

namespace tson
{
    class Tileset;

    class Tile
    {
        public:
            inline Tile() = default;
            inline Tile(IJson &json, tson::Tileset *tileset, tson::Map *map);
            inline Tile(uint32_t id, tson::Tileset *tileset, tson::Map *map);
            inline Tile(uint32_t id, tson::Map *map); //v1.2.0
            inline bool parse(IJson &json, tson::Tileset *tileset, tson::Map *map);
            inline bool parseId(IJson &json);


            [[nodiscard]] inline uint32_t getId() const;

            [[nodiscard]] inline const fs::path &getImage() const;

            [[nodiscard]] inline const Vector2i &getImageSize() const;
            [[nodiscard]] inline const std::string &getType() const;

            //[[nodiscard]] inline const std::vector<tson::Frame> &getAnimation() const;
            [[nodiscard]] inline tson::Animation &getAnimation();
            [[nodiscard]] inline const Layer &getObjectgroup() const;
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline const std::vector<int> &getTerrain() const;

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0-stuff
            inline void setProperties(const tson::PropertyCollection &properties);

            inline tson::Tileset * getTileset() const;
            inline tson::Map * getMap() const;
            inline const tson::Rect &getDrawingRect() const;
            inline const tson::Vector2f getPosition(const std::tuple<int, int> &tileDataPos);
            inline const tson::Vector2i getPositionInTileUnits(const std::tuple<int, int> &tileDataPos);
            inline const tson::Vector2i getTileSize() const;                       /*! Declared in tileson_forward.hpp */

            [[nodiscard]] inline TileFlipFlags getFlipFlags() const;
            inline bool hasFlipFlags(TileFlipFlags flags);
            [[nodiscard]] inline uint32_t getGid() const;

            inline void addTilesetAndPerformCalculations(tson::Tileset *tileset); //v1.2.0


        private:
            tson::Animation                  m_animation{};      /*! 'animation': Array of Frames */
            uint32_t                         m_id {};            /*! 'id': Local ID of the tile */

            fs::path                         m_image;            /*! 'image': Image representing this tile (optional)*/

            tson::Vector2i                   m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            tson::Layer                      m_objectgroup; 	 	/*! 'objectgroup': Layer with type objectgroup (optional) */
            tson::PropertyCollection         m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
            std::vector<int>                 m_terrain;          /*! 'terrain': Index of terrain for each corner of tile */
            std::string                      m_type;             /*! 'type': The type of the tile (optional) */

            //v1.2.0-stuff
            uint32_t                    m_gid {};                                    /*! id without flip flags */
            tson::Tileset *             m_tileset;                                   /*! A pointer to the tileset where this Tile comes from */
            tson::Map *                 m_map;                                       /*! A pointer to the map where this tile is contained */
            tson::Rect                  m_drawingRect;                               /*! A rect that shows which part of the tileset that is used for this tile */
            tson::TileFlipFlags         m_flipFlags = TileFlipFlags::None;           /*! Resolved using bit 32, 31 and 30 from gid */
            inline void performDataCalculations();                                   /*! Declared in tileson_forward.hpp - Calculate all the values used in the tile class. */
            inline void manageFlipFlagsByIdThenRemoveFlags(uint32_t &id);
            friend class Layer;
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Tile::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

tson::Tile::Tile(IJson &json, tson::Tileset *tileset, tson::Map *map)
{
    parse(json, tileset, map);
}

/*!
 * Used in cases where you have a tile without any property
 * @param id
 */
tson::Tile::Tile(uint32_t id, tson::Tileset *tileset, tson::Map *map) : m_id {id}, m_gid {id}
{
    m_tileset = tileset;
    m_map = map;
    manageFlipFlagsByIdThenRemoveFlags(m_gid);
    performDataCalculations();
}

/*!
 * Used in cases where you have a FLIP FLAGGED tile
 * @param id
 */
tson::Tile::Tile(uint32_t id, tson::Map *map) : m_id {id}, m_gid {id}
{
    m_map = map;
    manageFlipFlagsByIdThenRemoveFlags(m_gid);
}

/*!
 * For flip flagged tiles, tilesets must be resolved later.
 * @param tileset
 */
void tson::Tile::addTilesetAndPerformCalculations(tson::Tileset *tileset)
{
    m_tileset = tileset;
    performDataCalculations();
}

/*!
 * Parses a tile from a Tiled json. id on tile is store as id + 1 to match the references in data containers.
 * @param json
 * @return
 */
bool tson::Tile::parse(IJson &json, tson::Tileset *tileset, tson::Map *map)
{
    m_tileset = tileset;
    m_map = map;

    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional

    bool allFound = parseId(json);

    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional
    if(json.count("objectgroup") > 0) m_objectgroup = tson::Layer(json["objectgroup"], m_map); //Optional

    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional

    //More advanced data
    if(json.count("animation") > 0 && json["animation"].isArray())
    {
        auto &animation = json.array("animation");
        std::vector<tson::Frame> frames;
        std::for_each(animation.begin(), animation.end(), [&](std::unique_ptr<IJson> &item) { frames.emplace_back(*item); });
        if(frames.size() > 0)
        {
            m_animation.setFrames(frames);
        }
    }
    if(json.count("terrain") > 0 && json["terrain"].isArray())
    {
        auto &terrain = json.array("terrain");
        std::for_each(terrain.begin(), terrain.end(), [&](std::unique_ptr<IJson> &item) { m_terrain.emplace_back(item->get<int>()); });
    }

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item); });
    }

    performDataCalculations();

    return allFound;
}

/*!
 * 'id': Local ID of the tile
 * @return
 */
uint32_t tson::Tile::getId() const
{
    return m_id;
}

/*!
 * 'image': Image representing this tile (optional)
 * @return
 */

const fs::path &tson::Tile::getImage() const { return m_image; }

/*!
 * x = 'imagewidth' and y = 'imageheight': in pixels
 * @return
 */
const tson::Vector2i &tson::Tile::getImageSize() const
{
    return m_imageSize;
}

/*!
 * 'type': The type of the tile (optional)
 * @return
 */
const std::string &tson::Tile::getType() const
{
    return m_type;
}

/*!
 * 'animation': Array of Frames
 * @return
 */
tson::Animation &tson::Tile::getAnimation()
{
    return m_animation;
}

/*!
 * 'objectgroup': Layer with type objectgroup (optional)
 * @return
 */
const tson::Layer &tson::Tile::getObjectgroup() const
{
    return m_objectgroup;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Tile::getProperties()
{
    return m_properties;
}

/*!
 * 'terrain': Index of terrain for each corner of tile
 * @return
 */
const std::vector<int> &tson::Tile::getTerrain() const
{
    return m_terrain;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Tile::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);

    return nullptr;
}

/*!
 * Used for getting the tson::Tileset who is the parent of this Tile.
 * @return a pointer to the tson::Tileset where this tile is contained.
 */
tson::Tileset *tson::Tile::getTileset() const
{
    return m_tileset;
}

/*!
 * Used for getting the tson::Map who is the parent of this Tile.
 * @return a pointer to the tson::Map where this tile is contained.
 */
tson::Map *tson::Tile::getMap() const
{
    return m_map;
}

/*!
 * Get the information needed to draw the Tile based on its current tileset
 * @return a tson::Rect containing the information needed to draw the tile.
 */
const tson::Rect &tson::Tile::getDrawingRect() const
{
    return m_drawingRect;
}

/*!
 * Helper function.
 *
 * Get the position of the tile in tile units.
 * The size of each unit is determined by the tile size property of the map.
 * Example: If the tile size is 16x16 in the map, a tile unit of [2, 4] would be [32, 64] in pixels.
 * If you want the position in pixels: use getPosition() instead.
 *
 * @return Position of tile in tile units.
 */
const tson::Vector2i tson::Tile::getPositionInTileUnits(const std::tuple<int, int> &tileDataPos)
{
    return {std::get<0>(tileDataPos), std::get<1>(tileDataPos)};
}

void tson::Tile::manageFlipFlagsByIdThenRemoveFlags(uint32_t &id)
{
    if (id & FLIPPED_HORIZONTALLY_FLAG) m_flipFlags |= TileFlipFlags::Horizontally;
    if (id & FLIPPED_VERTICALLY_FLAG) m_flipFlags |= TileFlipFlags::Vertically;
    if (id & FLIPPED_DIAGONALLY_FLAG) m_flipFlags |= TileFlipFlags::Diagonally;

    id &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
}

tson::TileFlipFlags tson::Tile::getFlipFlags() const
{
    return m_flipFlags;
}

/*!
 *
 * @param flags Which flags to check for. Several flags can be checked at once using the bitwise or operator.
 * Example:
 * hasFlipFlags(TileFlipFlags::Vertically | TileFlipFlags::Horizontally)
 *
 * @return true if the flag(s) specified are set
 */
bool tson::Tile::hasFlipFlags(tson::TileFlipFlags flags)
{
    return ((m_flipFlags & flags) == flags) ? true : false;
}

uint32_t tson::Tile::getGid() const
{
    return m_gid;
}

void tson::Tile::setProperties(const tson::PropertyCollection &properties)
{
    m_properties = properties;
}


#endif //TILESON_TILE_HPP

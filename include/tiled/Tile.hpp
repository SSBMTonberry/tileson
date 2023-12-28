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
    class TiledClass;

    class Tile
    {
        public:
            inline Tile() = default;
            inline Tile(IJson &json, tson::Tileset *tileset, tson::Map *map);
            inline static Tile CreateFlippedTile(uint32_t gid, tson::Map *map); // v1.2.0
            inline static Tile CreateMissingTile(uint32_t id, tson::Tileset* tileset, tson::Map* map);
            inline bool parse(IJson &json, tson::Tileset *tileset, tson::Map *map);
            inline bool parseId(IJson &json);

            [[nodiscard]] inline uint32_t getId() const;
            [[nodiscard]] inline const fs::path &getImage() const;
            [[nodiscard]] inline const Vector2i &getImageSize() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */

            //[[nodiscard]] inline const std::vector<tson::Frame> &getAnimation() const;
            [[nodiscard]] inline tson::Animation &getAnimation();
            [[nodiscard]] inline Layer &getObjectgroup();
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline const std::vector<int> &getTerrain() const;

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0-stuff
            inline void setProperties(const tson::PropertyCollection &properties);

            [[nodiscard]] inline tson::Tileset * getTileset() const;
            [[nodiscard]] inline tson::Map * getMap() const;
            [[nodiscard]] inline const tson::Rect &getDrawingRect() const;
            [[nodiscard]] inline const Rect &getSubRectangle() const;

            inline const tson::Vector2f getPosition(const std::tuple<int, int> &tileDataPos);
            inline const tson::Vector2i getPositionInTileUnits(const std::tuple<int, int> &tileDataPos);
            [[nodiscard]] inline const tson::Vector2i getTileSize() const;                       /*! Declared in tileson_forward.hpp */

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
            tson::Rect                  m_subRect;                                   /*! Tiled 1.9: Contains the newly added sub-rectangle variables: 'x', 'y', 'width' and 'height'*/
            tson::TileFlipFlags         m_flipFlags = TileFlipFlags::None;           /*! Resolved using bit 32, 31 and 30 from gid */
            inline void performDataCalculations();                                   /*! Declared in tileson_forward.hpp - Calculate all the values used in the tile class. */
            inline void manageFlipFlagsByIdThenRemoveFlags(uint32_t &id);
            friend class Layer;
            std::shared_ptr<tson::TiledClass> m_class {};
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
 * For flip flagged tiles, tilesets must be resolved later.
 * @param tileset
 */
void tson::Tile::addTilesetAndPerformCalculations(tson::Tileset *tileset)
{
    m_tileset = tileset;
    performDataCalculations();
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
 * This was renamed to 'class' in Tiled v1.9
 * @return
 */
const std::string &tson::Tile::getType() const
{
    return m_type;
}

/*!
  * 'class': String assigned to class field in editor (optional)
 * This was renamed from 'type' to 'class' in Tiled v1.9
 * @return
 */
const std::string &tson::Tile::getClassType() const
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
tson::Layer &tson::Tile::getObjectgroup()
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

/*!
 * Tiled 1.9: Contains the newly added sub-rectangle variables: 'x', 'y', 'width' and 'height'
 * @return A tson::Rect with the 'x', 'y', 'width' and 'height' values
 */
const tson::Rect &tson::Tile::getSubRectangle() const
{
    return m_subRect;
}


#endif //TILESON_TILE_HPP

//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILE_HPP
#define TILESON_TILE_HPP

#include "../external/json.hpp"
#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "Frame.hpp"
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
            inline explicit Tile(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map);
            inline explicit Tile(int id, tson::Tileset *tileset, tson::Map *map);
            inline bool parse(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map);

            [[nodiscard]] inline int getId() const;
            #ifndef DISABLE_CPP17_FILESYSTEM
            [[nodiscard]] inline const fs::path &getImage() const;
            #else
            [[nodiscard]] inline const std::string &getImage() const;
            #endif
            [[nodiscard]] inline const Vector2i &getImageSize() const;
            [[nodiscard]] inline const std::string &getType() const;

            [[nodiscard]] inline const std::vector<tson::Frame> &getAnimation() const;
            [[nodiscard]] inline const Layer &getObjectgroup() const;
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline const std::vector<int> &getTerrain() const;

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0-stuff
            inline tson::Tileset * getTileset() const;
            inline tson::Map * getMap() const;
            inline const tson::Rect &getDrawingRect() const;
            inline const tson::Vector2f getPosition(const std::tuple<int, int> &tileDataPos);
            inline const tson::Vector2i getPositionInTileUnits(const std::tuple<int, int> &tileDataPos);
            inline const tson::Vector2i getTileSize() const;                       /*! Declared in tileson_forward.hpp */

        private:
            std::vector<tson::Frame>    m_animation; 	    /*! 'animation': Array of Frames */
            int                         m_id {};            /*! 'id': Local ID of the tile */
            #ifndef DISABLE_CPP17_FILESYSTEM
            fs::path                    m_image;            /*! 'image': Image representing this tile (optional)*/
            #else
            std::string                 m_image;
            #endif
            tson::Vector2i              m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            tson::Layer                 m_objectgroup; 	 	/*! 'objectgroup': Layer with type objectgroup (optional) */
            tson::PropertyCollection    m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
            std::vector<int>            m_terrain;          /*! 'terrain': Index of terrain for each corner of tile */
            std::string                 m_type;             /*! 'type': The type of the tile (optional) */

            //v1.2.0-stuff
            tson::Tileset *             m_tileset;                                   /*! A pointer to the tileset where this Tile comes from */
            tson::Map *                 m_map;                                       /*! A pointer to the map where this tile is contained */
            tson::Rect                  m_drawingRect;                               /*! A rect that shows which part of the tileset that is used for this tile */
            inline void performDataCalculations();                                   /*! Declared in tileson_forward.hpp - Calculate all the values used in the tile class. */

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

tson::Tile::Tile(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map)
{
    parse(json, tileset, map);
}

/*!
 * Used in cases where you have a tile without any property
 * @param id
 */
tson::Tile::Tile(int id, tson::Tileset *tileset, tson::Map *map) : m_id {id}
{
    m_tileset = tileset;
    m_map = map;
    performDataCalculations();
}

/*!
 * Parses a tile from a Tiled json. id on tile is store as id + 1 to match the references in data containers.
 * @param json
 * @return
 */
bool tson::Tile::parse(const nlohmann::json &json, tson::Tileset *tileset, tson::Map *map)
{
    m_tileset = tileset;
    m_map = map;

    bool allFound = true;
    #ifndef DISABLE_CPP17_FILESYSTEM
    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
    #else
    if(json.count("image") > 0) m_image = json["image"].get<std::string>(); //Optional
    #endif
    if(json.count("id") > 0) m_id = json["id"].get<int>() + 1; else allFound = false;
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional
    if(json.count("objectgroup") > 0) m_objectgroup = tson::Layer(json["objectgroup"], m_map); //Optional

    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional

    //More advanced data
    if(json.count("animation") > 0 && json["animation"].is_array())
        std::for_each(json["animation"].begin(), json["animation"].end(), [&](const nlohmann::json &item) { m_animation.emplace_back(item); });
    if(json.count("terrain") > 0 && json["terrain"].is_array())
        std::for_each(json["terrain"].begin(), json["terrain"].end(), [&](const nlohmann::json &item) { m_terrain.emplace_back(item.get<int>()); });

    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    performDataCalculations();

    return allFound;
}

/*!
 * 'id': Local ID of the tile
 * @return
 */
int tson::Tile::getId() const
{
    return m_id;
}

/*!
 * 'image': Image representing this tile (optional)
 * @return
 */
#ifndef DISABLE_CPP17_FILESYSTEM
const fs::path &tson::Tile::getImage() const { return m_image; }
#else
const std::string &tson::Tile::getImage() const { return m_image; }
#endif
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
const std::vector<tson::Frame> &tson::Tile::getAnimation() const
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


#endif //TILESON_TILE_HPP

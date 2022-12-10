//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_LAYER_HPP
#define TILESON_LAYER_HPP

#include <set>
//#include "../external/json.hpp"
#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "Chunk.hpp"
#include "Object.hpp"
#include "../objects/TileObject.hpp"
#include "../objects/Property.hpp"
#include "../objects/PropertyCollection.hpp"
#include "../common/Enums.hpp"
#include "../objects/FlaggedTile.hpp"

namespace tson
{
    class Tile;
    class Map;

    class Layer
    {
        public:
            inline Layer() = default;
            inline Layer(IJson &json, tson::Map *map);
            inline bool parse(IJson &json, tson::Map *map); //Defined in tileson_forward

            [[nodiscard]] inline const std::string &getCompression() const;
            [[nodiscard]] inline const std::vector<uint32_t> &getData() const;
            [[nodiscard]] inline const std::string &getBase64Data() const;
            [[nodiscard]] inline const std::string &getDrawOrder() const;
            [[nodiscard]] inline const std::string &getEncoding() const;
            [[nodiscard]] inline int getId() const;
            [[nodiscard]] inline const std::string &getImage() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline const Vector2f &getOffset() const;
            [[nodiscard]] inline float getOpacity() const;
            [[nodiscard]] inline const Vector2i &getSize() const;
            [[nodiscard]] inline const Colori &getTransparentColor() const;
            [[nodiscard]] inline const Vector2f &getParallax() const;
            [[nodiscard]] inline bool hasRepeatX() const;
            [[nodiscard]] inline bool hasRepeatY() const;

            [[nodiscard]] inline LayerType getType() const;
            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */

            [[nodiscard]] inline const std::string &getTypeStr() const;
            [[nodiscard]] inline bool isVisible() const;
            [[nodiscard]] inline int getX() const;
            [[nodiscard]] inline int getY() const;

            [[nodiscard]] inline std::vector<tson::Chunk> &getChunks();
            [[nodiscard]] inline std::vector<tson::Layer> &getLayers();
            [[nodiscard]] inline std::vector<tson::Object> &getObjects();
            [[nodiscard]] inline PropertyCollection &getProperties();

            inline tson::Object *getObj(int id);
            inline tson::Object *firstObj(const std::string &name);
            inline std::vector<tson::Object> getObjectsByName(const std::string &name);
            inline std::vector<tson::Object> getObjectsByType(tson::ObjectType type);

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            inline void assignTileMap(std::map<uint32_t, tson::Tile*> *tileMap);
            inline void createTileData(const Vector2i &mapSize, bool isInfiniteMap);

            [[nodiscard]] inline const std::map<std::tuple<int, int>, tson::Tile *> &getTileData() const;
            inline tson::Tile * getTileData(int x, int y);

            //v1.2.0-stuff
            [[nodiscard]] inline const Colori &getTintColor() const;
            [[nodiscard]] inline tson::Map *getMap() const;

            [[nodiscard]] inline std::map<std::tuple<int, int>, tson::TileObject> &getTileObjects();
            inline tson::TileObject * getTileObject(int x, int y);
            [[nodiscard]] inline const std::set<uint32_t> &getUniqueFlaggedTiles() const;
            inline void resolveFlaggedTiles();

        private:
            inline void setTypeByString();

            std::vector<tson::Chunk>                       m_chunks; 	                      /*! 'chunks': Array of chunks (optional). tilelayer only. */
            std::string                                    m_compression;                     /*! 'compression': zlib, gzip or empty (default). tilelayer only. */
            std::vector<uint32_t>                          m_data;                            /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded
                                                                                               *   data. tilelayer only. */
            std::string                                    m_base64Data;                      /*! 'data' (when string):     Array of unsigned int (GIDs) or base64-encoded
                                                                                               *   data. tilelayer only. */
            std::string                                    m_drawOrder;                       /*! 'draworder': topdown (default) or index. objectgroup only. */
            std::string                                    m_encoding;                        /*! 'encoding': csv (default) or base64. tilelayer only. */
            int                                            m_id{};                            /*! 'id': Incremental id - unique across all layers */
            std::string                                    m_image;                           /*! 'image': Image used by this layer. imagelayer only. */
            std::vector<tson::Layer>                       m_layers; 	                      /*! 'layers': Array of layers. group on */
            std::string                                    m_name;                            /*! 'name': Name assigned to this layer */
            std::vector<tson::Object>                      m_objects;                         /*! 'objects': Array of objects. objectgroup only. */
            tson::Vector2f                                 m_offset;                          /*! 'offsetx' and 'offsety': Horizontal and Vertical layer offset in pixels
                                                                                               *  (default: {0, 0}) */
            float                                          m_opacity{};                       /*! 'opacity': Value between 0 and 1 */
            tson::PropertyCollection                       m_properties; 	                  /*! 'properties': A list of properties (name, value, type). */
            tson::Vector2i                                 m_size;                            /*! x = 'width': (Column count. Same as map width for fixed-size maps.)
                                                                                                  y = 'height': Row count. Same as map height for fixed-size maps. */
            tson::Colori                                   m_transparentColor;                /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional, imagelayer only */
            std::string                                    m_typeStr;                         /*! 'type': tilelayer, objectgroup, imagelayer or group */
            LayerType                                      m_type {LayerType::Undefined};     /*! Layer type as enum*/
            bool                                           m_visible{};                       /*! 'visible': Whether layer is shown or hidden in editor */
            int                                            m_x{};                             /*! 'x': Horizontal layer offset in tiles. Always 0. */
            int                                            m_y{};                             /*! 'y': Vertical layer offset in tiles. Always 0. */
            tson::Vector2f                                 m_parallax{1.f, 1.f};    /*! Tiled v1.5: parallax factor for this layer. Defaults to 1.
                                                                                                  x = 'parallaxx', y = 'parallaxy'*/
            bool                                           m_repeatX {};                         /*! 'repeatx': Whether the image drawn by this layer is repeated along the X axis. (since Tiled 1.8)*/
            bool                                           m_repeatY {};                         /*! 'repeaty': Whether the image drawn by this layer is repeated along the Y axis. (since Tiled 1.8)*/

            std::map<uint32_t, tson::Tile*>                *m_tileMap;
            std::map<std::tuple<int, int>, tson::Tile*>    m_tileData;                        /*! Key: Tuple of x and y pos in tile units. */

            //v1.2.0-stuff
            tson::Colori                                        m_tintColor;                  /*! 'tintcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) that is multiplied with
                                                                                               *        any graphics drawn by this layer or any child layers (optional). */
            inline void decompressData();                                                     /*! Defined in tileson_forward.hpp */
            inline void queueFlaggedTile(size_t x, size_t y, uint32_t id);                    /*! Queue a flagged tile */

            tson::Map *                                         m_map;                        /*! The map who owns this layer */
            std::map<std::tuple<int, int>, tson::TileObject>    m_tileObjects;
            std::set<uint32_t>                                  m_uniqueFlaggedTiles;
            std::vector<tson::FlaggedTile>                      m_flaggedTiles;

            std::string                                         m_classType{};              /*! 'class': The class of this map (since 1.9, defaults to “”). */
            std::shared_ptr<tson::TiledClass>                   m_class {};
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T Layer::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

/*!
 * Parses a Tiled layer from json
 * @param json
 */
tson::Layer::Layer(IJson &json, tson::Map *map)
{
    parse(json, map);
}

void tson::Layer::queueFlaggedTile(size_t x, size_t y, uint32_t id)
{
    uint32_t tileId = id;
    tileId &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
    m_uniqueFlaggedTiles.insert(id);
    m_flaggedTiles.emplace_back(x, y, id, tileId);
}

/*!
 * Copies all objects with a name that equals the parameter
 * @param name Name of the objects to return
 * @return All objects with a matching name
 */
std::vector<tson::Object> tson::Layer::getObjectsByName(const std::string &name)
{
    std::vector<tson::Object> found;

    std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(found), [&](const tson::Object &item)
    {
        return item.getName() == name;
    });

    return found;
}



/*!
 * Copies all objects with a type that equals the parameter
 * @param type LayerType of the objects to return
 * @return All objects with a matching type
 */
std::vector<tson::Object> tson::Layer::getObjectsByType(tson::ObjectType type)
{
    std::vector<tson::Object> found;

    std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(found), [&](const tson::Object &item)
    {
        return item.getObjectType() == type;
    });

    return found;
}

/*!
 * Returns the first object with the given name
 * @param name Name of the object to find.
 * @return A pointer to the object if found. nullptr otherwise.
 */
tson::Object *tson::Layer::firstObj(const std::string &name)
{
    auto result = std::find_if(m_objects.begin(), m_objects.end(), [&](const tson::Object &obj){return obj.getName() == name; });
    if(result == m_objects.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Get an object by ID
 * @param id Unique ID of the object
 * @return A pointer to the object if found. nullptr otherwise.
 */
tson::Object *tson::Layer::getObj(int id)
{
    auto result = std::find_if(m_objects.begin(), m_objects.end(), [&](const tson::Object &obj){return obj.getId() == id; });
    if(result == m_objects.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Set type by string
 * tilelayer, objectgroup, imagelayer or group
 */
void tson::Layer::setTypeByString()
{
    if(m_typeStr == "tilelayer") m_type = LayerType::TileLayer;
    else if(m_typeStr == "objectgroup") m_type = LayerType::ObjectGroup;
    else if(m_typeStr == "imagelayer") m_type = LayerType::ImageLayer;
    else if(m_typeStr == "group") m_type = LayerType::Group;
    else m_type = LayerType::Undefined;
}

/*!
 * 'compression': zlib, gzip or empty (default). tilelayer only.
 * @return
 */
const std::string &tson::Layer::getCompression() const
{
    return m_compression;
}

/*!
 * 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::vector<uint32_t> &tson::Layer::getData() const
{
    return m_data;
}

/*!
 * 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getBase64Data() const
{
    return m_base64Data;
}

/*!
 * 'draworder': topdown (default) or index. objectgroup only.
 * @return
 */
const std::string &tson::Layer::getDrawOrder() const
{
    return m_drawOrder;
}

/*!
 * 'encoding': csv (default) or base64. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getEncoding() const
{
    return m_encoding;
}

/*!
 * 'id': Incremental id - unique across all layers
 * @return
 */
int tson::Layer::getId() const
{
    return m_id;
}

/*!
 * 'image': Image used by this layer. imagelayer only.
 * @return
 */
const std::string &tson::Layer::getImage() const
{
    return m_image;
}

/*!
 * 'name': Name assigned to this layer
 * @return
 */
const std::string &tson::Layer::getName() const
{
    return m_name;
}

/*!
 * 'offsetx' and 'offsety': Horizontal and Vertical layer offset in pixels (default: {0, 0})
 * @return
 */
const tson::Vector2f &tson::Layer::getOffset() const
{
    return m_offset;
}

/*!
 * 'opacity': Value between 0 and 1
 * @return
 */
float tson::Layer::getOpacity() const
{
    return m_opacity;
}

/*!
 * x = 'width': (Column count. Same as map width for fixed-size maps.)
 * y = 'height': Row count. Same as map height for fixed-size maps.
 * @return width and height as a single size
 */
const tson::Vector2i &tson::Layer::getSize() const
{
    return m_size;
}

/*!
 * 'transparentcolor': Color created from a hex color (#RRGGBB) (optional, imagelayer only)
 * @return color as color object with rgba channel.
 */
const tson::Colori &tson::Layer::getTransparentColor() const
{
    return m_transparentColor;
}

/*!
 * 'type': tilelayer, objectgroup, imagelayer or group
 * @return string with the object type
 */
const std::string &tson::Layer::getTypeStr() const
{
    return m_typeStr;
}

/*!
 * 'visible': Whether layer is shown or hidden in editor
 * @return
 */
bool tson::Layer::isVisible() const
{
    return m_visible;
}

/*!
 * 'x': Horizontal layer offset in tiles. Always 0.
 * @return x value (always 0 for layer)
 */
int tson::Layer::getX() const
{
    return m_x;
}

/*!
 * 'y': Horizontal layer offset in tiles. Always 0.
 * @return y value (always 0 for layer)
 */
int tson::Layer::getY() const
{
    return m_y;
}

/*!
 * 'chunks': Array of chunks (optional). tilelayer only.
 * @return
 */
std::vector<tson::Chunk> &tson::Layer::getChunks()
{
    return m_chunks;
}

/*!
 * 'layers': Array of layers. group on
 * @return
 */
std::vector<tson::Layer> &tson::Layer::getLayers()
{
    return m_layers;
}

/*!
 * 'objects': Array of objects. objectgroup only.
 * @return
 */
std::vector<tson::Object> &tson::Layer::getObjects()
{
    return m_objects;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Layer::getProperties()
{
    return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Layer::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

/*!
 * Get layer type
 * @return Layer type as enum
 */
tson::LayerType tson::Layer::getType() const
{
    return m_type;
}

/*!
 * Assigns a tilemap of pointers to existing tiles.
 * @param tileMap The tilemap. key: tile id, value: pointer to Tile.
 */
void tson::Layer::assignTileMap(std::map<uint32_t, tson::Tile *> *tileMap)
{
    m_tileMap = tileMap;
}

/*!
 * Get tile data as some kind of map with x and y position with pointers to existing tiles.
 * Map only contains tiles that are not empty. x and y position is in tile units.
 *
 * Example of getting tile from the returned map:
 *
 * Tile *tile = tileData[{0, 4}];
 *
 * @return A map that represents the data returned from getData() in a 2D map with Tile pointers.
 */
const std::map<std::tuple<int, int>, tson::Tile *> &tson::Layer::getTileData() const
{
    return m_tileData;
}

/*!
 * A safe way to get tile data
 * Get tile data as some kind of map with x and y position with pointers to existing tiles.
 * Map only contains tiles that are not empty. x and y position is in tile units.
 *
 * Example of getting tile:
 * Tile *tile = layer->getTileData(0, 4)
 *
 * @param x X position in tile units
 * @param y Y position in tile units
 * @return pointer to tile, if it exists. nullptr otherwise.
 */
tson::Tile *tson::Layer::getTileData(int x, int y)
{
    return (m_tileData.count({x, y}) > 0) ? m_tileData[{x,y}] : nullptr;
}

/*!
 * Used for getting the tson::Map who is the parent of this Layer.
 * @return a pointer to the tson::Map where this layer is contained.
 */
tson::Map *tson::Layer::getMap() const
{
    return m_map;
}

/*!
 *
 * This is only supported for non-infinite maps!
 *
 * @param mapSize The size of the map
 * @param isInfiniteMap Whether or not the current map is infinte.
 */
void tson::Layer::createTileData(const Vector2i &mapSize, bool isInfiniteMap)
{
    size_t x = 0;
    size_t y = 0;
    if(!isInfiniteMap)
    {
        std::for_each(m_data.begin(), m_data.end(), [&](uint32_t tileId)
        {
            if (static_cast<int>(x) == mapSize.x)
            {
                ++y;
                x = 0;
            }

            if (tileId > 0 && m_tileMap->count(tileId) > 0)
            {
                m_tileData[{static_cast<int>(x), static_cast<int>(y)}] = m_tileMap->at(tileId);
                m_tileObjects[{static_cast<int>(x), static_cast<int>(y)}] = {{static_cast<int>(x), static_cast<int>(y)}, m_tileData[{static_cast<int>(x), static_cast<int>(y)}]};
            }
            else if(tileId > 0 && m_tileMap->count(tileId) == 0) //Tile with flip flags!
            {
                queueFlaggedTile(x, y, tileId);
            }
            x++;
        });

    }
}

std::map<std::tuple<int, int>, tson::TileObject> &tson::Layer::getTileObjects()
{
    return m_tileObjects;
}

tson::TileObject *tson::Layer::getTileObject(int x, int y)
{
    return (m_tileObjects.count({x, y}) > 0) ? &m_tileObjects[{x,y}] : nullptr;
}

const std::set<uint32_t> &tson::Layer::getUniqueFlaggedTiles() const
{
    return m_uniqueFlaggedTiles;
}

void tson::Layer::resolveFlaggedTiles()
{
    std::for_each(m_flaggedTiles.begin(), m_flaggedTiles.end(), [&](const tson::FlaggedTile &tile)
    {
        if (tile.id > 0 && m_tileMap->count(tile.id) > 0)
        {
            m_tileData[{static_cast<int>(tile.x), static_cast<int>(tile.y)}] = m_tileMap->at(tile.id);
            m_tileObjects[{static_cast<int>(tile.x), static_cast<int>(tile.y)}] = {{static_cast<int>(tile.x), static_cast<int>(tile.y)}, m_tileData[{static_cast<int>(tile.x), static_cast<int>(tile.y)}]};
        }
    });
}

/*!
 * 'tintcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) that is multiplied with any graphics drawn by this layer or any child layers (optional).
 *
 * @return tintcolor
 */
const tson::Colori &tson::Layer::getTintColor() const
{
    return m_tintColor;
}

/*!
 * New in Tiled v1.5
 * Gets the parallax factor for current layer. Defaults to 1.
 * @return A vector with the x and y values of the parallax factor.
 */
const tson::Vector2f &tson::Layer::getParallax() const
{
    return m_parallax;
}

/*!
 * New in Tiled v1.8
 * 'repeatx': Whether the image drawn by this layer is repeated along the X axis.
 * @return true if image layer is repeated along the X axis, false otherwise.
 */
bool tson::Layer::hasRepeatX() const
{
    return m_repeatX;
}

/*!
 * New in Tiled v1.8
 * 'repeatx': Whether the image drawn by this layer is repeated along the Y axis.
 * @return true if image layer is repeated along the Y axis, false otherwise.
 */
bool tson::Layer::hasRepeatY() const
{
    return m_repeatY;
}

const std::string &tson::Layer::getClassType() const
{
    return m_classType;
}


#endif //TILESON_LAYER_HPP

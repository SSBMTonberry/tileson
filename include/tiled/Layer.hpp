//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_LAYER_HPP
#define TILESON_LAYER_HPP

#include "../external/json.hpp"
#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "Chunk.hpp"
#include "Object.hpp"
#include "../objects/TileObject.hpp"
#include "../objects/Property.hpp"
#include "../objects/PropertyCollection.hpp"
#include "../common/Enums.hpp"

namespace tson
{
    class Tile;
    class Map;

    class Layer
    {
        public:
            inline Layer() = default;
            inline Layer(const nlohmann::json &json, tson::Map *map);
            inline bool parse(const nlohmann::json &json, tson::Map *map);

            [[nodiscard]] inline const std::string &getCompression() const;
            [[nodiscard]] inline const std::vector<int> &getData() const;
            [[nodiscard]] inline const std::string &getBase64Data() const;
            [[nodiscard]] inline const std::string &getDrawOrder() const;
            [[nodiscard]] inline const std::string &getEncoding() const;
            [[nodiscard]] inline int getId() const;
            [[nodiscard]] inline const std::string &getImage() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline const Vector2f &getOffset() const;
            [[nodiscard]] inline float getOpacity() const;
            [[nodiscard]] inline const Vector2i &getSize() const;
            [[nodiscard]] inline const Colori &getTransparentcolor() const;

            [[nodiscard]] inline LayerType getType() const;

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

            inline void assignTileMap(const std::map<int, tson::Tile*> &tileMap);
            inline void createTileData(const Vector2i &mapSize, bool isInfiniteMap);

            [[nodiscard]] inline const std::map<std::tuple<int, int>, tson::Tile *> &getTileData() const;
            inline tson::Tile * getTileData(int x, int y);

            //v1.2.0-stuff
            inline tson::Map *getMap() const;

            [[nodiscard]] inline const std::map<std::tuple<int, int>, tson::TileObject> &getTileObjects() const;
            inline tson::TileObject * getTileObject(int x, int y);

        private:
            inline void setTypeByString();

            std::vector<tson::Chunk>                       m_chunks; 	                      /*! 'chunks': Array of chunks (optional). tilelayer only. */
            std::string                                    m_compression;                     /*! 'compression': zlib, gzip or empty (default). tilelayer only. */
            std::vector<int>                               m_data;                            /*! 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded
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
            tson::Colori                                   m_transparentcolor;                /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional, imagelayer only */
            std::string                                    m_typeStr;                         /*! 'type': tilelayer, objectgroup, imagelayer or group */
            LayerType                                      m_type {LayerType::Undefined};     /*! Layer type as enum*/
            bool                                           m_visible{};                       /*! 'visible': Whether layer is shown or hidden in editor */
            int                                            m_x{};                             /*! 'x': Horizontal layer offset in tiles. Always 0. */
            int                                            m_y{};                             /*! 'y': Vertical layer offset in tiles. Always 0. */

            std::map<int, tson::Tile*>                     m_tileMap;
            std::map<std::tuple<int, int>, tson::Tile*>    m_tileData;                        /*! Key: Tuple of x and y pos in tile units. */

            //v1.2.0-stuff
            inline void decompressData();                                                     /*! Defined in tileson_forward.hpp */
            tson::Map *                                         m_map;                        /*! The map who owns this layer */
            std::map<std::tuple<int, int>, tson::TileObject>    m_tileObjects;
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
tson::Layer::Layer(const nlohmann::json &json, tson::Map *map)
{
    parse(json, map);
}

/*!
 * Parses a Tiled layer from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Layer::parse(const nlohmann::json &json, tson::Map *map)
{
    m_map = map;

    bool allFound = true;
    if(json.count("compression") > 0) m_compression = json["compression"].get<std::string>(); //Optional
    if(json.count("draworder") > 0) m_drawOrder = json["draworder"].get<std::string>(); //Optional
    if(json.count("encoding") > 0) m_encoding = json["encoding"].get<std::string>(); //Optional
    if(json.count("id") > 0) m_id = json["id"].get<int>(); //Optional
    if(json.count("image") > 0) m_image = json["image"].get<std::string>(); //Optional
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("offsetx") > 0 && json.count("offsety") > 0)
        m_offset = {json["offsetx"].get<float>(), json["offsety"].get<float>()}; //Optional
    if(json.count("opacity") > 0) m_opacity = json["opacity"].get<float>(); else allFound = false;
    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("transparentcolor") > 0) m_transparentcolor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
    if(json.count("type") > 0) m_typeStr = json["type"].get<std::string>(); else allFound = false;
    if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;
    if(json.count("x") > 0) m_x = json["x"].get<int>(); else allFound = false;
    if(json.count("y") > 0) m_y = json["y"].get<int>(); else allFound = false;

    //Handle DATA (Optional)
    if(json.count("data") > 0)
    {
        if(json["data"].is_array())
        {
            std::for_each(json["data"].begin(), json["data"].end(), [&](const nlohmann::json &item) { m_data.push_back(item.get<int>()); });
        }
        else
        {
            m_base64Data = json["data"].get<std::string>();
            decompressData();
        }
    }

    //More advanced data
    if(json.count("chunks") > 0 && json["chunks"].is_array())
        std::for_each(json["chunks"].begin(), json["chunks"].end(), [&](const nlohmann::json &item) { m_chunks.emplace_back(item); });
    if(json.count("layers") > 0 && json["layers"].is_array())
        std::for_each(json["layers"].begin(), json["layers"].end(), [&](const nlohmann::json &item) { m_layers.emplace_back(item, m_map); });
    if(json.count("objects") > 0 && json["objects"].is_array())
        std::for_each(json["objects"].begin(), json["objects"].end(), [&](const nlohmann::json &item) { m_objects.emplace_back(item); });
    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    setTypeByString();

    return allFound;
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
const std::vector<int> &tson::Layer::getData() const
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
const tson::Colori &tson::Layer::getTransparentcolor() const
{
    return m_transparentcolor;
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
void tson::Layer::assignTileMap(const std::map<int, tson::Tile *> &tileMap)
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
 * This is only supported for non-infinte maps!
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
        std::for_each(m_data.begin(), m_data.end(), [&](int tileId)
        {
            if (x == mapSize.x)
            {
                ++y;
                x = 0;
            }

            if (tileId > 0)
            {
                m_tileData[{x, y}] = m_tileMap[tileId];
                m_tileObjects[{x, y}] = {{x, y}, m_tileData[{x, y}]};
            }
            x++;
        });
    }
}

const std::map<std::tuple<int, int>, tson::TileObject> &tson::Layer::getTileObjects() const
{
    return m_tileObjects;
}

tson::TileObject *tson::Layer::getTileObject(int x, int y)
{
    return (m_tileObjects.count({x, y}) > 0) ? &m_tileObjects[{x,y}] : nullptr;
}



#endif //TILESON_LAYER_HPP

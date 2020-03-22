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

namespace tson
{
    class Tile
    {
        public:
            inline Tile() = default;
            inline explicit Tile(const nlohmann::json &json);
            inline explicit Tile(int id);
            inline bool parse(const nlohmann::json &json);

            [[nodiscard]] inline int getId() const;
            #if USE_CPP17_FILESYSTEM
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

        private:
            std::vector<tson::Frame>    m_animation; 	    /*! 'animation': Array of Frames */
            int                         m_id {};            /*! 'id': Local ID of the tile */
            #if USE_CPP17_FILESYSTEM
            fs::path                    m_image;            /*! 'image': Image representing this tile (optional)*/
            #else
            std::string                 m_image;
            #endif
            tson::Vector2i              m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            tson::Layer                 m_objectgroup; 	 	/*! 'objectgroup': Layer with type objectgroup (optional) */
            tson::PropertyCollection    m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
            std::vector<int>            m_terrain;          /*! 'terrain': Index of terrain for each corner of tile */
            std::string                 m_type;             /*! 'type': The type of the tile (optional) */
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

tson::Tile::Tile(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Used in cases where you have a tile without any property
 * @param id
 */
tson::Tile::Tile(int id) : m_id {id}
{

}

/*!
 * Parses a tile from a Tiled json. id on tile is store as id + 1 to match the references in data containers.
 * @param json
 * @return
 */
bool tson::Tile::parse(const nlohmann::json &json)
{
    bool allFound = true;
    #if USE_CPP17_FILESYSTEM
    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
    #else
    if(json.count("image") > 0) m_image = json["image"].get<std::string>(); //Optional
    #endif
    if(json.count("id") > 0) m_id = json["id"].get<int>() + 1; else allFound = false;
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
#if USE_CPP17_FILESYSTEM
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

#endif //TILESON_TILE_HPP

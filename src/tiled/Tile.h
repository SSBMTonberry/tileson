//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_TILE_H
#define TILESON_TILE_H

#include <json.hpp>
#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "Frame.h"
#include "../objects/PropertyCollection.h"
#include "Layer.h"

namespace tson
{
    class Tile
    {
        public:
            Tile() = default;
            explicit Tile(const nlohmann::json &json);
            explicit Tile(int id);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] int getId() const;
            #if USE_CPP17_FILESYSTEM
            [[nodiscard]] const fs::path &getImage() const;
            #else
            [[nodiscard]] const std::string &getImage() const;
            #endif
            [[nodiscard]] const Vector2i &getImageSize() const;
            [[nodiscard]] const std::string &getType() const;

            [[nodiscard]] const std::vector<tson::Frame> &getAnimation() const;
            [[nodiscard]] const Layer &getObjectgroup() const;
            [[nodiscard]] PropertyCollection &getProperties();
            [[nodiscard]] const std::vector<int> &getTerrain() const;

            template <typename T>
            T get(const std::string &name);
            tson::Property * getProp(const std::string &name);

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

#endif //TILESON_TILE_H

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

#if MSVC
#include <filesystem>
    namespace fs = std::filesystem;
#elif MINGW
#if __MINGW64_VERSION_MAJOR > 6
        #include <filesystem>
        namespace fs = std::filesystem;
    #else
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #endif
#elif APPLE
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #if __GNUC__ < 8 //GCC major version less than 8
#include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #include <filesystem>
namespace fs = std::filesystem;
    #endif
#endif

namespace tson
{
    class Tile
    {
        public:
            Tile() = default;
            explicit Tile(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] int getId() const;
            [[nodiscard]] const fs::path &getImage() const;
            [[nodiscard]] const Vector2i &getImageSize() const;
            [[nodiscard]] const std::string &getType() const;

            [[nodiscard]] const std::vector<tson::Frame> &getAnimation() const;
            [[nodiscard]] const Layer &getObjectgroup() const;
            [[nodiscard]] PropertyCollection &getProperties();
            [[nodiscard]] const std::vector<int> &getTerrain() const;


        private:
            std::vector<tson::Frame>    m_animation; 	    /*! 'animation': Array of Frames */
            int                         m_id {};            /*! 'id': Local ID of the tile */
            fs::path                    m_image;            /*! 'image': Image representing this tile (optional)*/
            tson::Vector2i              m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            tson::Layer                 m_objectgroup; 	 	/*! 'objectgroup': Layer with type objectgroup (optional) */
            tson::PropertyCollection    m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
            std::vector<int>            m_terrain;          /*! 'terrain': Index of terrain for each corner of tile */
            std::string                 m_type;             /*! 'type': The type of the tile (optional) */
    };
}

#endif //TILESON_TILE_H

//
// Created by robin on 02.08.2019.
//

#ifndef TILESON_TILESET_H
#define TILESON_TILESET_H

#include <json.hpp>

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

#include "../objects/Vector2.hpp"
#include "../objects/Color.hpp"
#include "../objects/PropertyCollection.h"
#include "WangSet.h"
#include "Tile.h"
#include "Terrain.h"
#include "Grid.h"

namespace tson
{
    class Tileset
    {
        public:
            Tileset() = default;
            explicit Tileset(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] int getColumns() const;
            [[nodiscard]] int getFirstgid() const;
            [[nodiscard]] const fs::path &getImagePath() const;
            [[nodiscard]] const Vector2i &getImageSize() const;
            [[nodiscard]] int getMargin() const;
            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] int getSpacing() const;
            [[nodiscard]] int getTileCount() const;
            [[nodiscard]] const Vector2i &getTileSize() const;
            [[nodiscard]] const Colori &getTransparentColor() const;
            [[nodiscard]] const std::string &getType() const;

            [[nodiscard]] const fs::path &getImage() const;
            [[nodiscard]] const std::vector<tson::Tile> &getTiles() const;
            [[nodiscard]] const std::vector<tson::WangSet> &getWangsets() const;
            [[nodiscard]] PropertyCollection &getProperties();
            [[nodiscard]] const std::vector<tson::Terrain> &getTerrains() const;
            [[nodiscard]] const Vector2i &getTileOffset() const;
            [[nodiscard]] const Grid &getGrid() const;

        private:
            int                           m_columns {};       /*! 'columns': The number of tile columns in the tileset */
            int                           m_firstgid {};      /*! 'firstgid': GID corresponding to the first tile in the set */
            fs::path                      m_image;            /*! 'image': Image used for tiles in this set */
            tson::Vector2i                m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            int                           m_margin {};        /*! 'margin': Buffer between image edge and first tile (pixels)*/
            std::string                   m_name;             /*! 'name': Name given to this tileset */
            int                           m_spacing {};       /*! 'spacing': Spacing between adjacent tiles in image (pixels)*/
            int                           m_tileCount {};     /*! 'tilecount': The number of tiles in this tileset */
            tson::Vector2i                m_tileSize;         /*! x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set */
            tson::Colori                  m_transparentColor; /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional) */
            std::string                   m_type;             /*! 'type': tileset (for tileset files, since 1.0) */

            std::vector<tson::Tile>       m_tiles;            /*! 'tiles': Array of Tiles (optional) */
            std::vector<tson::WangSet>    m_wangsets;         /*! 'wangsets':Array of Wang sets (since 1.1.5) */
            tson::PropertyCollection      m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

            std::vector<tson::Terrain>    m_terrains;         /*! 'terrains': Array of Terrains (optional) */
            tson::Vector2i                m_tileOffset;       /*! 'x' and 'y': See <tileoffset> (optional) */
            tson::Grid                    m_grid;             /*! 'grid': This element is only used in case of isometric orientation, and determines
                                                                   how tile overlays for terrain and collision information are rendered. */
    };
}

#endif //TILESON_TILESET_H

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
#include "../objects/Vector2.hpp"
#include "../objects/Color.h"

namespace fs = std::filesystem;
    #endif
#endif


namespace tson
{
    class Tileset
    {
        public:
            Tileset() = default;
            explicit Tileset(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            int getColumns() const;
            int getFirstgid() const;
            const fs::path &getImagePath() const;
            const Vector2i &getImageSize() const;
            int getMargin() const;
            const std::string &getName() const;
            int getSpacing() const;
            int getTileCount() const;
            const Vector2i &getTileSize() const;
            const Color &getTransparentColor() const;
            const std::string &getType() const;

        private:
            int                     m_columns {};          /*! 'columns': The number of tile columns in the tileset */
            int                     m_firstgid {};         /*! 'firstgid': GID corresponding to the first tile in the set */
            //TODO: grid            object              See <grid> (optional)
            fs::path                m_image;            /*! 'image': Image used for tiles in this set */
            tson::Vector2i          m_imageSize;        /*! x = 'imagewidth' and y = 'imageheight': in pixels */
            int                     m_margin {};           /*! 'margin': Buffer between image edge and first tile (pixels)*/
            std::string             m_name;             /*! 'name': Name given to this tileset */
            //TODO: properties 	    array 	            A list of properties (name, value, type).
            int                     m_spacing {};          /*! 'spacing': Spacing between adjacent tiles in image (pixels)*/
            //TODO: terrains 	    array 	            Array of Terrains (optional)
            int                     m_tileCount {};        /*! 'tilecount': The number of tiles in this tileset */
            tson::Vector2i          m_tileSize;         /*! x = 'tilewidth' and y = 'tileheight': Maximum size of tiles in this set */
            //TODO: tileoffset 	    object 	            See <tileoffset> (optional)
            //TODO: tiles 	        array 	            Array of Tiles (optional)
            tson::Color             m_transparentColor; /*! 'transparentcolor': Hex-formatted color (#RRGGBB) (optional) */
            std::string             m_type;             /*! 'type': tileset (for tileset files, since 1.0) */
            //TODO: wangsets 	    array 	            Array of Wang sets (since 1.1.5)

            /*
             columns 	            int 	            The number of tile columns in the tileset
             firstgid 	            int 	            GID corresponding to the first tile in the set
             grid 	                object 	            See <grid> (optional)
             image 	                string 	            Image used for tiles in this set
             imagewidth 	        int 	            Width of source image in pixels
             imageheight 	        int 	            Height of source image in pixels
             margin 	            int 	            Buffer between image edge and first tile (pixels)
             name 	                string 	            Name given to this tileset
             properties 	        array 	            A list of properties (name, value, type).
             spacing 	            int 	            Spacing between adjacent tiles in image (pixels)
             terrains 	            array 	            Array of Terrains (optional)
             tilecount 	            int 	            The number of tiles in this tileset
             tileheight 	        int 	            Maximum height of tiles in this set
             tileoffset 	        object 	            See <tileoffset> (optional)
             tiles 	                array 	            Array of Tiles (optional)
             tilewidth 	            int 	            Maximum width of tiles in this set
             transparentcolor 	    string 	            Hex-formatted color (#RRGGBB) (optional)
             type 	                string 	            tileset (for tileset files, since 1.0)
             wangsets 	            array 	            Array of Wang sets (since 1.1.5)
             *
             *
             */
    };
}

#endif //TILESON_TILESET_H

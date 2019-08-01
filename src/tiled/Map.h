//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_MAP_H
#define TILESON_MAP_H

#include "../objects/Color.h"
#include "../objects/Vector2.hpp"
#include "json.hpp"

namespace tson
{
    class Map
    {
        public:
            Map() = default;
            explicit Map(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            const Color &getBackgroundColor() const;
            const Vector2i &getSize() const;
            int getHexsideLength() const;
            bool isInfinite() const;
            int getNextLayerId() const;
            int getNextObjectId() const;
            const std::string &getOrientation() const;
            const std::string &getRenderOrder() const;
            const std::string &getStaggerAxis() const;
            const std::string &getStaggerIndex() const;
            const std::string &getTiledVersion() const;
            const Vector2i &getTileSize() const;
            const std::string &getType() const;
            const std::string &getVersion() const;

        protected:
            Color       m_backgroundColor;   /*! 'backgroundcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) (optional)*/;
            Vector2i    m_size;              /*! 'width' and 'height' of a Tiled map */
            int         m_hexsideLength;     /*! 'hexsidelength': Length of the side of a hex tile in pixels */
            bool        m_isInfinite;        /*! 'infinite': Whether the map has infinite dimensions*/
            //TODO: layers      |      array      | 	Array of Layers
            int         m_nextLayerId;       /*! 'nextlayerid': Auto-increments for each layer */
            int         m_nextObjectId;      /*! 'nextobjectid': Auto-increments for each placed object */
            std::string m_orientation;       /*! 'orientation': orthogonal, isometric, staggered or hexagonal */
            //TODO: properties  |      array 	  |     A list of properties (name, value, type).
            std::string m_renderOrder;       /*! 'renderorder': Rendering direction (orthogonal maps only) */
            std::string m_staggerAxis;       /*! 'staggeraxis': x or y (staggered / hexagonal maps only) */
            std::string m_staggerIndex;      /*! 'staggerindex': odd or even (staggered / hexagonal maps only) */
            std::string m_tiledVersion;      /*! 'tiledversion': The Tiled version used to save the file */
            Vector2i    m_tileSize;          /*! 'tilewidth' and 'tileheight' of a map */
            //TODO: tilesets    |      array 	  |     Array of Tilesets
            std::string m_type;              /*! 'type': map (since 1.0) */
            std::string m_version;           /*! 'version': The JSON format version*/

        /*
         * backgroundcolor 	string 	Hex-formatted color (#RRGGBB or #AARRGGBB) (optional)
           height        	int 	Number of tile rows
           hexsidelength    int 	Length of the side of a hex tile in pixels
           infinite      	bool 	Whether the map has infinite dimensions
           layers        	array 	Array of Layers
           nextlayerid      int 	Auto-increments for each layer
           nextobjectid     int 	Auto-increments for each placed object
           orientation      string 	orthogonal, isometric, staggered or hexagonal
           properties       array 	A list of properties (name, value, type).
           renderorder      string 	Rendering direction (orthogonal maps only)
           staggeraxis      string 	x or y (staggered / hexagonal maps only)
           staggerindex     string 	odd or even (staggered / hexagonal maps only)
           tiledversion     string 	The Tiled version used to save the file
           tileheight       int 	Map grid height
           tilesets      	array 	Array of Tilesets
           tilewidth        int 	Map grid width
           type      	    string 	map (since 1.0)
           version       	number 	The JSON format version
           width         	int 	Number of tile columns
         */
    };
}

#endif //TILESON_MAP_H

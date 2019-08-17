//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_MAP_H
#define TILESON_MAP_H

#include "../objects/Color.hpp"
#include "../objects/Vector2.hpp"
#include "json.hpp"
#include "Layer.h"
#include "Tileset.h"

namespace tson
{
    class Map
    {
        public:
            enum class ParseStatus : uint8_t
            {
                OK = 0, //OK unless otherwise stated
                FileNotFound = 1,
                ParseError = 2,
                MissingData = 3
            };

            Map() = default;
            Map(ParseStatus status, std::string description);
            explicit Map(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] const Colori &getBackgroundColor() const;
            [[nodiscard]] const Vector2i &getSize() const;
            [[nodiscard]] int getHexsideLength() const;
            [[nodiscard]] bool isInfinite() const;
            [[nodiscard]] int getNextLayerId() const;
            [[nodiscard]] int getNextObjectId() const;
            [[nodiscard]] const std::string &getOrientation() const;
            [[nodiscard]] const std::string &getRenderOrder() const;
            [[nodiscard]] const std::string &getStaggerAxis() const;
            [[nodiscard]] const std::string &getStaggerIndex() const;
            [[nodiscard]] const std::string &getTiledVersion() const;
            [[nodiscard]] const Vector2i &getTileSize() const;
            [[nodiscard]] const std::string &getType() const;
            [[nodiscard]] int getVersion() const;

            [[nodiscard]] std::vector<tson::Layer> &getLayers();
            [[nodiscard]] PropertyCollection &getProperties();
            [[nodiscard]] std::vector<tson::Tileset> &getTilesets();

            [[nodiscard]] ParseStatus getStatus() const;
            [[nodiscard]] const std::string &getStatusMessage() const;
            [[nodiscard]] const std::map<int, tson::Tile *> &getTileMap() const;

            Layer * getLayer(const std::string &name);
            Tileset * getTileset(const std::string &name);

            template <typename T>
            T get(const std::string &name);
            tson::Property * getProp(const std::string &name);

        private:
            void processData();

            Colori                                 m_backgroundColor;   /*! 'backgroundcolor': Hex-formatted color (#RRGGBB or #AARRGGBB) (optional)*/;
            Vector2i                               m_size;              /*! 'width' and 'height' of a Tiled map */
            int                                    m_hexsideLength {};  /*! 'hexsidelength': Length of the side of a hex tile in pixels */
            bool                                   m_isInfinite {};     /*! 'infinite': Whether the map has infinite dimensions*/
            std::vector<tson::Layer>               m_layers; 	        /*! 'layers': Array of layers. group on */
            int                                    m_nextLayerId {};    /*! 'nextlayerid': Auto-increments for each layer */
            int                                    m_nextObjectId {};   /*! 'nextobjectid': Auto-increments for each placed object */
            std::string                            m_orientation;       /*! 'orientation': orthogonal, isometric, staggered or hexagonal */
            tson::PropertyCollection               m_properties; 	    /*! 'properties': A list of properties (name, value, type). */
            std::string                            m_renderOrder;       /*! 'renderorder': Rendering direction (orthogonal maps only) */
            std::string                            m_staggerAxis;       /*! 'staggeraxis': x or y (staggered / hexagonal maps only) */
            std::string                            m_staggerIndex;      /*! 'staggerindex': odd or even (staggered / hexagonal maps only) */
            std::string                            m_tiledVersion;      /*! 'tiledversion': The Tiled version used to save the file */
            Vector2i                               m_tileSize;          /*! 'tilewidth': and 'tileheight' of a map */
            std::vector<tson::Tileset>             m_tilesets;          /*! 'tilesets': Array of Tilesets */
            std::string                            m_type;              /*! 'type': map (since 1.0) */
            int                                    m_version{};         /*! 'version': The JSON format version*/

            ParseStatus                            m_status {ParseStatus::OK};
            std::string                            m_statusMessage {"OK"};

            std::map<int, tson::Tile*>             m_tileMap;           /*! key: Tile ID. Value: Pointer to Tile*/
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Map::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

#endif //TILESON_MAP_H

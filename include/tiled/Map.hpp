//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_MAP_HPP
#define TILESON_MAP_HPP

#include <functional>

#include "../objects/Color.hpp"
#include "../objects/Vector2.hpp"
//#include "../external/json.hpp"
#include "../interfaces/IJson.hpp"
#include "../json/NlohmannJson.hpp"
#include "../json/PicoJson.hpp"
//#include "../json/Gason.hpp" //Unsupported
#include "../json/Json11.hpp"

#include "Layer.hpp"
#include "Tileset.hpp"

#include "../common/Enums.hpp"

namespace tson
{
    using LinkedFileParser = std::function<std::unique_ptr<IJson>(std::string relativePath)>;
    class Object;
    class Map
    {
        friend class Object;
        public:
            inline Map() = default;
            inline Map(ParseStatus status, std::string description);
            inline explicit Map(IJson &json, tson::DecompressorContainer *decompressors, tson::Project *project, tson::LinkedFileParser linkedFileParser = nullptr);
            inline bool parse(IJson &json, tson::DecompressorContainer *decompressors, tson::Project *project, tson::LinkedFileParser linkedFileParser = nullptr);

            [[nodiscard]] inline const Colori &getBackgroundColor() const;
            [[nodiscard]] inline const Vector2i &getSize() const;
            [[nodiscard]] inline int getHexsideLength() const;
            [[nodiscard]] inline bool isInfinite() const;
            [[nodiscard]] inline int getNextLayerId() const;
            [[nodiscard]] inline int getNextObjectId() const;
            [[nodiscard]] inline const std::string &getOrientation() const;
            [[nodiscard]] inline const std::string &getRenderOrder() const;
            [[nodiscard]] inline const std::string &getStaggerAxis() const;
            [[nodiscard]] inline const std::string &getStaggerIndex() const;
            [[nodiscard]] inline const std::string &getTiledVersion() const;
            [[nodiscard]] inline const Vector2i &getTileSize() const;
            [[nodiscard]] inline const std::string &getType() const;
            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */
            [[nodiscard]] inline const Vector2f &getParallaxOrigin() const;
            //[[nodiscard]] inline int getVersion() const; //Removed - Tileson v1.3.0

            [[nodiscard]] inline std::vector<tson::Layer> &getLayers();
            [[nodiscard]] inline PropertyCollection &getProperties();
            [[nodiscard]] inline std::vector<tson::Tileset> &getTilesets();

            [[nodiscard]] inline ParseStatus getStatus() const;
            [[nodiscard]] inline const std::string &getStatusMessage() const;
            [[nodiscard]] inline const std::map<uint32_t, tson::Tile *> &getTileMap() const;

            inline Layer * getLayer(const std::string &name);
            inline Tileset * getTileset(const std::string &name);

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            //v1.2.0
            [[nodiscard]] inline int getCompressionLevel() const;
            inline DecompressorContainer *getDecompressors();
            inline Project * getProject();
            inline Tileset * getTilesetByGid(uint32_t gid);


        private:
            inline IJson* parseLinkedFile(const std::string& path);
            inline bool createTilesetData(IJson &json);
            inline void processData();

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
            tson::Vector2f                         m_parallaxOrigin;    /*! Tiled v1.8: parallax origin in pixels. Defaults to 0. */
            //int                                    m_version{};       /*! 'version': The JSON format version - Removed in Tileson v1.3.0*/

            ParseStatus                            m_status {ParseStatus::OK};
            std::string                            m_statusMessage {"OK"};

            std::map<uint32_t, tson::Tile*>        m_tileMap{};           /*! key: Tile ID. Value: Pointer to Tile*/

            //v1.2.0
            int                                    m_compressionLevel {-1};  /*! 'compressionlevel': The compression level to use for tile layer
                                                                              *     data (defaults to -1, which means to use the algorithm default)
                                                                              *     Introduced in Tiled 1.3*/
            tson::DecompressorContainer *          m_decompressors {nullptr};
            tson::Project *                        m_project {nullptr};
            std::map<uint32_t, tson::Tile>         m_flaggedTileMap{};    /*! key: Tile ID. Value: Tile*/

            std::string                            m_classType{};              /*! 'class': The class of this map (since 1.9, defaults to “”). */
            std::shared_ptr<tson::TiledClass>      m_class {};

            tson::LinkedFileParser                 m_linkedFileParser;     /*! callback function to parse linked files */
            std::map<std::string, std::unique_ptr<IJson>> m_linkedFiles;  /*! key: relative path to linked file. Value: Pointer to loaded JSON */
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

/*!
 * When errors have happened before the map starts parsing, just keep the statuses
 * @param status The status
 * @param description Description of the status
 */
tson::Map::Map(tson::ParseStatus status, std::string description) : m_status {status}, m_statusMessage { std::move(description) }
{

}

/*!
 * Parses a json of a Tiled map.
 * @param json A json object with the format of Map
 * @param linkedFileParser A callback function that must return a IJson object when a linked file is found in the map
 * @return true if all mandatory fields was found. false otherwise.
 */
tson::Map::Map(IJson &json, tson::DecompressorContainer *decompressors, tson::Project *project, tson::LinkedFileParser linkedFileParser)
{
    parse(json, decompressors, project, linkedFileParser);
}

/*!
 * Parses a json of a Tiled map.
 * @param json A json object with the format of Map
 * @param linkedFileParser A callback function that must return a IJson object when a linked file is found in the map
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Map::parse(IJson &json, tson::DecompressorContainer *decompressors, tson::Project *project, tson::LinkedFileParser linkedFileParser)
{
    m_decompressors = decompressors;
    m_project = project;
    m_linkedFileParser = linkedFileParser;

    if(!m_linkedFileParser)
    {  // build a default linked file parser out of processing relative paths to
       // the main json's location.
        m_linkedFileParser = [&json](std::string relativePath) -> std::unique_ptr<IJson>
        {
            if(json.directory().empty())
                return nullptr;
            
            std::unique_ptr<IJson> linkedFileJson = json.create();
            bool parseOk = linkedFileJson->parse(json.directory() / relativePath);
            if(parseOk)
                return linkedFileJson;
            else
                return nullptr;
        };
    }

    bool allFound = true;
    if(json.count("compressionlevel") > 0)
        m_compressionLevel = json["compressionlevel"].get<int>(); //Tiled 1.3 - Optional

    if(json.count("backgroundcolor") > 0) m_backgroundColor = Colori(json["backgroundcolor"].get<std::string>()); //Optional
    if(json.count("width") > 0 && json.count("height") > 0 )
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("hexsidelength") > 0) m_hexsideLength = json["hexsidelength"].get<int>();         //Optional
    if(json.count("infinite") > 0) m_isInfinite = json["infinite"].get<bool>();                     //Optional
    if(json.count("nextlayerid") > 0) m_nextLayerId = json["nextlayerid"].get<int>();               //Optional
    if(json.count("nextobjectid") > 0) m_nextObjectId = json["nextobjectid"].get<int>(); else allFound = false;
    if(json.count("orientation") > 0) m_orientation = json["orientation"].get<std::string>(); else allFound = false;
    if(json.count("renderorder") > 0) m_renderOrder = json["renderorder"].get<std::string>();       //Optional
    if(json.count("staggeraxis") > 0) m_staggerAxis = json["staggeraxis"].get<std::string>();       //Optional
    if(json.count("staggerindex") > 0) m_staggerIndex = json["staggerindex"].get<std::string>();    //Optional
    if(json.count("tiledversion") > 0) m_tiledVersion = json["tiledversion"].get<std::string>(); else allFound = false;
    if(json.count("tilewidth") > 0 && json.count("tileheight") > 0 )
        m_tileSize = {json["tilewidth"].get<int>(), json["tileheight"].get<int>()}; else allFound = false;
    if(json.count("type") > 0) m_type = json["type"].get<std::string>();                            //Optional
    if(json.count("class") > 0) m_classType = json["class"].get<std::string>();                     //Optional

    //Removed - Changed from a float to string in Tiled v1.6, and old spec said int.
    //Reason for removal is that it seems to have no real use, as TiledVersion is stored in another variable.
    //if(json.count("version") > 0) m_version = json["version"].get<int>(); else allFound = false;

    //More advanced data
    if(json.count("layers") > 0 && json["layers"].isArray())
    {
        auto &array = json.array("layers");
        std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item)
        {
            m_layers.emplace_back(*item, this);
        });
    }

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &array = json.array("properties");
        std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item)
        {
            m_properties.add(*item, m_project);
        });
    }

    tson::Vector2f parallaxOrigin {0.f, 0.f};
    if(json.count("parallaxoriginx") > 0)
        parallaxOrigin.x = json["parallaxoriginx"].get<float>();
    if(json.count("parallaxoriginy") > 0)
        parallaxOrigin.y = json["parallaxoriginy"].get<float>();

    m_parallaxOrigin = parallaxOrigin;

    if(!createTilesetData(json))
        allFound = false;

    processData();

    m_linkedFiles.clear(); // close all open linked json files

    return allFound;
}

/*!
 * Attempts to parse a linked file found in the map.
 * @param relativePath Path to the linked file, relative to the map file.
 * @return a IJson pointer to the parsed file.
 */
tson::IJson* tson::Map::parseLinkedFile(const std::string& relativePath) 
{
    auto it = m_linkedFiles.find(relativePath);
    if(it == m_linkedFiles.end())
    {
        if (!m_linkedFileParser) return nullptr;
        std::unique_ptr<IJson> linkedFileJson = m_linkedFileParser(relativePath);
        if(!linkedFileJson)
            return nullptr;

        auto result = m_linkedFiles.emplace(relativePath, std::move(linkedFileJson));
        return result.first->second.get();
    }
    else return it->second.get();
}

/*!
 * Tileset data must be created in two steps to prevent malformed tson::Tileset pointers inside tson::Tile
 */
bool tson::Map::createTilesetData(IJson &json)
{
    bool ok = true;
    if(json.count("tilesets") > 0 && json["tilesets"].isArray())
    {
        //First created tileset objects
        auto &tilesets = json.array("tilesets");
        std::for_each(tilesets.begin(), tilesets.end(), [&](std::unique_ptr<IJson> &)
        {
            m_tilesets.emplace_back();
        });

        int i = 0;
        //Then do the parsing
        std::for_each(tilesets.begin(), tilesets.end(), [&](std::unique_ptr<IJson> &item)
        {
            item->directory(json.directory());
            if(!m_tilesets[i].parse(*item, this))
                ok = false;

            ++i;
        });
    }
    return ok;
}

/*!
 * Processes the parsed data and uses the data to create helpful objects, like tile maps.
 */
void tson::Map::processData()
{
    m_tileMap.clear();
    for(auto &tileset : m_tilesets)
    {
          std::set<std::uint32_t> usedIds;
          for(auto& tile : tileset.getTiles())
          {
              if (usedIds.count(tile.getGid()) != 0)
              {
                  continue;
              }
              usedIds.insert(tile.getGid());
              m_tileMap[tile.getGid()] = &tile;
          }
    }
    std::for_each(m_layers.begin(), m_layers.end(), [&](tson::Layer &layer)
    {
        layer.assignTileMap(&m_tileMap);
        layer.createTileData(m_size, m_isInfinite);
        const std::set<uint32_t> &flaggedTiles = layer.getUniqueFlaggedTiles();
        for(uint32_t ftile : flaggedTiles)
        {
            tson::Tile tile = tson::Tile::CreateFlippedTile(ftile, layer.getMap());
            if(m_tileMap.count(tile.getGid()))
            {
                tson::Tile *originalTile = m_tileMap[tile.getGid()];
                tile.addTilesetAndPerformCalculations(originalTile->getTileset());
                tile.setProperties(originalTile->getProperties());
                m_flaggedTileMap[ftile] = tile;
                m_tileMap[ftile] = &m_flaggedTileMap[ftile];
            }
        }
        layer.resolveFlaggedTiles();
    });
}

/*!
 * 'backgroundcolor': Color created from a hex-formatted color string (#RRGGBB or #AARRGGBB) (optional)
 * @return string as color
 */
const tson::Colori &tson::Map::getBackgroundColor() const
{
    return m_backgroundColor;
}

/*!
 * 'width' and 'height' of a Tiled map
 * @return
 */
const tson::Vector2<int> &tson::Map::getSize() const
{
    return m_size;
}

/*!
 * 'hexsidelength': Length of the side of a hex tile in pixels
 * @return
 */
int tson::Map::getHexsideLength() const
{
    return m_hexsideLength;
}

/*!
 * 'infinite': Whether the map has infinite dimensions
 * @return
 */
bool tson::Map::isInfinite() const
{
    return m_isInfinite;
}

/*!
 * 'nextlayerid': Auto-increments for each layer
 * @return
 */
int tson::Map::getNextLayerId() const
{
    return m_nextLayerId;
}

/*!
 * 'nextobjectid': Auto-increments for each placed object
 * @return
 */
int tson::Map::getNextObjectId() const
{
    return m_nextObjectId;
}

/*!
 * 'orientation': orthogonal, isometric, staggered or hexagonal
 * @return
 */
const std::string &tson::Map::getOrientation() const
{
    return m_orientation;
}

/*!
 * 'renderorder': Rendering direction (orthogonal maps only)
 * @return
 */
const std::string &tson::Map::getRenderOrder() const
{
    return m_renderOrder;
}

/*!
 * 'staggeraxis': x or y (staggered / hexagonal maps only)
 * @return
 */
const std::string &tson::Map::getStaggerAxis() const
{
    return m_staggerAxis;
}

/*!
 * 'staggerindex': odd or even (staggered / hexagonal maps only)
 * @return
 */
const std::string &tson::Map::getStaggerIndex() const
{
    return m_staggerIndex;
}

/*!
 * 'tiledversion': The Tiled version used to save the file
 * @return
 */
const std::string &tson::Map::getTiledVersion() const
{
    return m_tiledVersion;
}

/*!
 * 'tilewidth': and 'tileheight' of a map
 * @return
 */
const tson::Vector2<int> &tson::Map::getTileSize() const
{
    return m_tileSize;
}

/*!
 * 'type': map (since 1.0)
 * @return
 */
const std::string &tson::Map::getType() const
{
    return m_type;
}

/*!
 * 'version': The JSON format version
 * @return
 */
//int tson::Map::getVersion() const
//{
//    return m_version;
//}

/*!
 * 'layers': Array of layers. group on
 * @return
 */
std::vector<tson::Layer> &tson::Map::getLayers()
{
    return m_layers;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Map::getProperties()
{
    return m_properties;
}

/*!
 * 'tilesets': Array of Tilesets
 * @return
 */
std::vector<tson::Tileset> &tson::Map::getTilesets()
{
    return m_tilesets;
}

tson::Layer *tson::Map::getLayer(const std::string &name)
{
    auto result = std::find_if(m_layers.begin(), m_layers.end(), [&](const tson::Layer &item) { return item.getName() == name; });
    if(result == m_layers.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Gets a tileset by name
 *
 * @param name Name of the tileset
 * @return tileset with the matching name
 */
tson::Tileset *tson::Map::getTileset(const std::string &name)
{
    auto result = std::find_if(m_tilesets.begin(), m_tilesets.end(), [&](const tson::Tileset &item) {return item.getName() == name; });
    if(result == m_tilesets.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Gets a tileset by gid (graphical ID of a tile). These are always unique, no matter how many tilesets you have
 *
 * @param gid Graphical ID of a tile
 * @return tileset related to the actual gid
 */
tson::Tileset *tson::Map::getTilesetByGid(uint32_t gid)
{
    auto result = std::find_if(m_tilesets.begin(), m_tilesets.end(), [&](const tson::Tileset &tileset)
    {
        auto const firstId = static_cast<uint32_t>(tileset.getFirstgid()); //First tile id of the tileset
        auto const lastId =  static_cast<uint32_t>((firstId + tileset.getTileCount()) - 1);

        return (gid >= firstId && gid <= lastId);
    });
    if(result == m_tilesets.end())
        return nullptr;

    return &result.operator*();
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Map::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

tson::ParseStatus tson::Map::getStatus() const
{
    return m_status;
}

const std::string &tson::Map::getStatusMessage() const
{
    return m_statusMessage;
}

/*!
 * Get a tile map with pointers to every existing tile.
 * @return
 */
const std::map<uint32_t, tson::Tile *> &tson::Map::getTileMap() const
{
    return m_tileMap;
}

tson::DecompressorContainer *tson::Map::getDecompressors()
{
    return m_decompressors;
}

/*!
 * 'compressionlevel': The compression level to use for tile layer data (defaults to -1, which means to use the algorithm default)
 *
 * @return The compression level
 */
int tson::Map::getCompressionLevel() const
{
    return m_compressionLevel;
}

/*!
 * New in Tiled v1.8
 * Gets the parallax origin in pixels. Defaults to 0.
 * @return A vector with the x and y values of the parallax origin.
 */
const tson::Vector2f &tson::Map::getParallaxOrigin() const
{
    return m_parallaxOrigin;
}

tson::Project *tson::Map::getProject()
{
    return m_project;
}

const std::string &tson::Map::getClassType() const
{
    return m_classType;
}


#endif //TILESON_MAP_HPP

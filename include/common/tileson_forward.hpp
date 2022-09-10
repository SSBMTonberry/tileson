//
// Created by robin on 25.07.2020.
//

#ifndef TILESON_TILESON_FORWARD_HPP
#define TILESON_TILESON_FORWARD_HPP
/*!
 * T I L E S O N   F O R W A R D   D E C L A R A T I O N S
 * -------------------------------------------------------
 *
 * Due to cross-references we have forward declarations that cannot be resolved during the
 * implementation, thus the implementations must be done later when the class definition itself is known.
 *
 * All those forward declarations can be found below.
 */


// T i l e . h p p
// ---------------------

/*!
 * Really just a shortcut to retrieve the tile size from the map.
 * @return TileSize based on the map property for tile size.
 */
const tson::Vector2i tson::Tile::getTileSize() const
{
    if(m_map != nullptr)
        return m_map->getTileSize();
    else
        return {0,0};
}

bool tson::Tile::parseId(IJson &json)
{
    if(json.count("id") > 0)
    {
        m_id = json["id"].get<uint32_t>() + 1;
        if (m_tileset != nullptr)
            m_gid = m_tileset->getFirstgid() + m_id - 1;
        else
            m_gid = m_id;
        manageFlipFlagsByIdThenRemoveFlags(m_gid);
        return true;
    }
    return false;
}

/*!
 * Uses tson::Tileset and tson::Map data to calculate related values for tson::Tile.
 * Added in v1.2.0
 */
void tson::Tile::performDataCalculations()
{
    if(m_tileset == nullptr || m_map == nullptr)
        return;

    int firstId = m_tileset->getFirstgid(); //First tile id of the tileset
    int columns = m_tileset->getColumns();
    int rows = m_tileset->getTileCount() / columns;
    int lastId = (m_tileset->getFirstgid() + m_tileset->getTileCount()) - 1;

    int const gid = static_cast<int>(getGid());
    if (gid >= firstId && gid <= lastId)
    {
        int const baseTilePosition = (gid - firstId);

        int const tileModX = (baseTilePosition % columns);
        int const currentRow = (baseTilePosition / columns);
        int const offsetX = (tileModX != 0) ? ((tileModX) * m_map->getTileSize().x) : (0 * m_map->getTileSize().x);
        int const offsetY =  (currentRow < rows-1) ? (currentRow * m_map->getTileSize().y) : ((rows-1) * m_map->getTileSize().y);

        tson::Vector2i spacing = m_tileset->getMarginSpacingOffset({tileModX, currentRow});
        m_drawingRect = { offsetX + spacing.x, offsetY + spacing.y, m_map->getTileSize().x, m_map->getTileSize().y };
    }
    else
        m_drawingRect = {0, 0, 0, 0};
}

/*!
 * Get the position of the tile in pixels based on the tile data position from the current layer.
 * @return The position of the tile in Pixels
 */
const tson::Vector2f tson::Tile::getPosition(const std::tuple<int, int> &tileDataPos)
{
    return {((float) std::get<0>(tileDataPos)) * m_drawingRect.width, ((float) std::get<1>(tileDataPos)) * m_drawingRect.height};
}

// T i l e O b j e c t . h p p
// ---------------------

/*!
 * In cases where the empty constructor is called, this must be called manually
 * for this class to make sense
 * @param posInTileUnits
 * @param tile
 */
void tson::TileObject::initialize(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile)
{
    m_tile = tile;
    m_posInTileUnits = tile->getPositionInTileUnits(posInTileUnits);
    m_position = tile->getPosition(posInTileUnits);
}

const tson::Rect &tson::TileObject::getDrawingRect() const
{
    return m_tile->getDrawingRect();
}

// L a y e r . h p p
// -------------------

/*!
 * Decompresses data if there are matching decompressors
 */
void tson::Layer::decompressData()
{

    tson::DecompressorContainer *container = m_map->getDecompressors();
    if(container->empty())
        return;

    if(m_encoding.empty() && m_compression.empty())
        return;

    std::string data = m_base64Data;
    bool hasBeenDecoded = false;
    if(!m_encoding.empty() && container->contains(m_encoding))
    {
        data = container->get(m_encoding)->decompress(data);
        hasBeenDecoded = true;
    }

    if(!m_compression.empty() && container->contains(m_compression))
    {
        data = container->get(m_compression)->decompress(data);
    }

    if(hasBeenDecoded)
    {
        std::vector<uint8_t> bytes = tson::Tools::Base64DecodedStringToBytes(data);
        m_data = tson::Tools::BytesToUnsignedInts(bytes);
    }
}

// W o r l d . h p p
// ------------------

/*!
 * Loads the actual maps based on the world data.
 * @param parser A Tileson object used for parsing the maps of the world.
 * @return How many maps who were parsed. Remember to call getStatus() for the actual map to find out if everything went okay.
 */

std::size_t tson::World::loadMaps(tson::Tileson *parser)
{
    m_maps.clear();
    std::for_each(m_mapData.begin(), m_mapData.end(), [&](const tson::WorldMapData &data)
    {
        if(fs::exists(data.path))
        {
            std::unique_ptr<tson::Map> map = parser->parse(data.path);
            m_maps.push_back(std::move(map));
        }
    });

    return m_maps.size();
}

// P r o p e r t y . h p p
// ------------------
void tson::Property::setValueByType(IJson &json)
{
    switch(m_type)
    {
        case Type::Color:
            m_value = Colori(json.get<std::string>());
            break;

        case Type::File:
            m_value = fs::path(json.get<std::string>());
            break;

        case Type::Int:
            if(!m_propertyType.empty())
            {
                m_type = Type::Enum;
                tson::EnumDefinition *def = (m_project != nullptr) ? m_project->getEnumDefinition(m_propertyType) : nullptr;
                if(def != nullptr)
                {
                    uint32_t v = json.get<uint32_t>();
                    m_value = tson::EnumValue(v, def);
                }
                else
                    m_value = tson::EnumValue();
            }
            else
                m_value = json.get<int>();

            break;

        case Type::Boolean:
            m_value = json.get<bool>();
            break;

        case Type::Float:
            m_value = json.get<float>();
            break;

        case Type::String:
            if(!m_propertyType.empty())
            {
                m_type = Type::Enum;
                tson::EnumDefinition *def = (m_project != nullptr) ? m_project->getEnumDefinition(m_propertyType) : nullptr;
                if(def != nullptr)
                {
                    std::string v = json.get<std::string>();
                    m_value = tson::EnumValue(v, def);
                }
                else
                    m_value = tson::EnumValue();
            }
            else
                setStrValue(json.get<std::string>());

            break;

        case Type::Class:
        {
            tson::TiledClass *baseClass = (m_project != nullptr) ? m_project->getClass(m_propertyType) : nullptr;
            if (baseClass != nullptr)
            {
                tson::TiledClass c = *baseClass;
                if(json.count("value") > 0)
                {
                    tson::IJson &v = json["value"]; //Contains an actual json with only values that differs from the original class.
                    //RBP: Create logic to correctly parse the values from the json and overwrite them in the class.
                    c.update(v);
                }
                m_value = c;
            }
        }
        break;

        case Type::Object:
            m_value = json.get<uint32_t>();
            break;
        default:
            setStrValue(json.get<std::string>());
            break;
    }
}


#endif //TILESON_TILESON_FORWARD_HPP

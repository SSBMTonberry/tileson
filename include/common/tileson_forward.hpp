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

// M a p . h p p
// ----------------

tson::TiledClass *tson::Map::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_project != nullptr) ? m_project->getClass(m_classType) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
}

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

/*!
 * Gets the class information for the 'type'/'class'
 * This may only give a valid result if the map is loaded through a tson::Project
 * @return a tson::TiledClass object if related map was loaded through tson::Project
 */
tson::TiledClass *tson::Tile::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_type) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
}

// T i l e s e t . h p p
// ------------------------

tson::TiledClass *tson::Tileset::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_classType) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
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

/*!
 * Parses a Tiled layer from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Layer::parse(IJson &json, tson::Map *map)
{
    m_map = map;

    bool allFound = true;
    if(json.count("tintcolor") > 0) m_tintColor = tson::Colori(json["tintcolor"].get<std::string>()); //Optional
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
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; //else allFound = false; - Not mandatory for all layers!
    if(json.count("transparentcolor") > 0) m_transparentColor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
    if(json.count("type") > 0) m_typeStr = json["type"].get<std::string>(); else allFound = false;
    if(json.count("class") > 0) m_classType = json["class"].get<std::string>();                     //Optional
    if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;
    if(json.count("x") > 0) m_x = json["x"].get<int>(); else allFound = false;
    if(json.count("y") > 0) m_y = json["y"].get<int>(); else allFound = false;
    if(json.count("repeatx") > 0) m_repeatX = json["repeatx"].get<bool>(); //Optional
    if(json.count("repeaty") > 0) m_repeatY = json["repeaty"].get<bool>(); //Optional

    tson::Vector2f parallax {1.f, 1.f};
    if(json.count("parallaxx") > 0)
        parallax.x = json["parallaxx"].get<float>();
    if(json.count("parallaxy") > 0)
        parallax.y = json["parallaxy"].get<float>();

    m_parallax = parallax;

    //Handle DATA (Optional)
    if(json.count("data") > 0)
    {
        if(json["data"].isArray())
        {
            auto &array = json.array("data");
            std::for_each(array.begin(), array.end(), [&](std::unique_ptr<IJson> &item) { m_data.push_back(item->get<uint32_t>()); });
        }
        else
        {
            m_base64Data = json["data"].get<std::string>();
            decompressData();
        }
    }

    //More advanced data
    if(json.count("chunks") > 0 && json["chunks"].isArray())
    {
        auto &chunks = json.array("chunks");
        std::for_each(chunks.begin(), chunks.end(), [&](std::unique_ptr<IJson> &item) { m_chunks.emplace_back(*item); });
    }
    if(json.count("layers") > 0 && json["layers"].isArray())
    {
        auto &layers = json.array("layers");
        std::for_each(layers.begin(), layers.end(), [&](std::unique_ptr<IJson> &item) { m_layers.emplace_back(*item, m_map); });
    }
    if(json.count("objects") > 0 && json["objects"].isArray())
    {
        auto &objects = json.array("objects");
        std::for_each(objects.begin(), objects.end(), [&](std::unique_ptr<IJson> &item) { m_objects.emplace_back(*item, m_map); });
    }
    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        tson::Project *project = (m_map != nullptr) ? m_map->getProject() : nullptr;
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item, project); });
    }

    setTypeByString();

    return allFound;
}

tson::TiledClass *tson::Layer::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_classType) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
}

// O b j e c t . h p p
// --------------------

// W a n g s e t . h p p
// ----------------------
tson::TiledClass *tson::WangSet::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_classType) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
}

// W a n g c o l o r . h p p
// ----------------------
tson::TiledClass *tson::WangColor::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_classType) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
}

/*!
 * Gets the class information for the 'type'/'class'
 * This may only give a valid result if the map is loaded through a tson::Project
 * @return a tson::TiledClass object if related map was loaded through tson::Project
 */
tson::TiledClass *tson::Object::getClass()
{
    if(m_class == nullptr)
    {
        TiledClass* baseClass = (m_map != nullptr && m_map->getProject() != nullptr) ? m_map->getProject()->getClass(m_type) : nullptr;
        if(baseClass != nullptr)
        {
            m_class = std::make_shared<TiledClass>(*baseClass);
            m_class->update(m_properties);
        }
    }
    return (m_class != nullptr) ? m_class.get() : nullptr;
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
                c.update(json);
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

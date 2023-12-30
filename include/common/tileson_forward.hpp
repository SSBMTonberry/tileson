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
 * Used in cases where you have a FLIP FLAGGED tile
 * @param id
 */
tson::Tile tson::Tile::CreateFlippedTile(uint32_t gid, tson::Map* map)
{
    // Parse flip flags and remove flip bits from global id
    tson::Tile tile;
    tile.manageFlipFlagsByIdThenRemoveFlags(gid);
        
    tile.m_map = map;
    tile.m_gid = gid;    
    
    // Compute local id from global id
    tile.m_id = tile.m_gid - tile.m_map->getTilesetByGid(tile.m_gid)->getFirstgid() + 1;
    
    return tile;
}

/*!
 * Used in cases where you have a tile without any property
 * @param id
 */
tson::Tile tson::Tile::CreateMissingTile(uint32_t id, tson::Tileset* tileset, tson::Map* map)
{
    Tile tile;
    tile.m_tileset = tileset;
    tile.m_map = map;
    tile.m_id = id;
    tile.m_gid = tileset->getFirstgid() + id - 1;
    tile.performDataCalculations();

    return tile;
}

/*!
 * Parses a tile from a Tiled json. id on tile is store as id + 1 to match the references in data containers.
 * @param json
 * @return
 */
bool tson::Tile::parse(IJson &json, tson::Tileset *tileset, tson::Map *map)
{
    m_tileset = tileset;
    m_map = map;
    
    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
    
    bool allFound = parseId(json);
    
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional
    else if(json.count("class") > 0) m_type = json["class"].get<std::string>(); //Tiled v1.9 renamed 'type' to 'class'
    
    if(json.count("objectgroup") > 0) m_objectgroup = tson::Layer(json["objectgroup"], m_map); //Optional
    
    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional
    
    m_subRect = {0,0, m_imageSize.x, m_imageSize.y};
    if(json.count("x") > 0) m_subRect.x = json["x"].get<int>(); //Optional
    if(json.count("y") > 0) m_subRect.y = json["y"].get<int>(); //Optional
    if(json.count("width") > 0) m_subRect.width = json["width"].get<int>(); //Optional
    if(json.count("height") > 0) m_subRect.height = json["height"].get<int>(); //Optional
    
    //More advanced data
    if(json.count("animation") > 0 && json["animation"].isArray())
    {
        auto &animation = json.array("animation");
        std::vector<tson::Frame> frames;
        std::for_each(animation.begin(), animation.end(), [&](std::unique_ptr<IJson> &item) { frames.emplace_back(*item); });
        if(frames.size() > 0)
        {
            m_animation.setFrames(frames);
        }
    }
    if(json.count("terrain") > 0 && json["terrain"].isArray())
    {
        auto &terrain = json.array("terrain");
        std::for_each(terrain.begin(), terrain.end(), [&](std::unique_ptr<IJson> &item) { m_terrain.emplace_back(item->get<int>()); });
    }
    
    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        tson::Project *project = (m_map != nullptr) ? m_map->getProject() : nullptr;
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item, project); });
    }
    
    performDataCalculations();
    
    return allFound;
}

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
    if (m_tileset == nullptr || m_map == nullptr)
        return;

    m_drawingRect = { 0, 0, 0, 0 };

    if (m_tileset->getType() == TilesetType::ImageTileset)
    {
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
            int const offsetX = (tileModX != 0) ? ((tileModX)*m_map->getTileSize().x) : (0 * m_map->getTileSize().x);
            int const offsetY = (currentRow < rows - 1) ? (currentRow * m_map->getTileSize().y) : ((rows - 1) * m_map->getTileSize().y);

            tson::Vector2i spacing = m_tileset->getMarginSpacingOffset({ tileModX, currentRow });
            m_drawingRect = { offsetX + spacing.x, offsetY + spacing.y, m_tileset->getTileSize().x, m_tileset->getTileSize().y };
        }
    }
    else if (m_tileset->getType() == TilesetType::ImageCollectionTileset)
    {
        tson::Vector2i imageSize = m_imageSize;

        // Tile in Image Collection Tileset contains image size
        if (getFlipFlags() != tson::TileFlipFlags::None)
        {
            uint32_t id = getGid() - m_tileset->getFirstgid() + 1;
            imageSize = m_tileset->getTile(id)->getImageSize();
        }

        m_drawingRect = { 0, 0, imageSize.x, imageSize.y };
    }
}

/*!
 * Get the position of the tile in pixels based on the tile data position from the current layer.
 * @return The position of the tile in Pixels
 */
const tson::Vector2f tson::Tile::getPosition(const std::tuple<int, int> &tileDataPos)
{
    tson::Vector2i tileSize = m_map->getTileSize();
    return {((float) std::get<0>(tileDataPos)) * tileSize.x, ((float) std::get<1>(tileDataPos)) * tileSize.y};
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

tson::Project *tson::Tileset::getProject() const {
    return m_map ? m_map->getProject() : nullptr;
}

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


/*!
* Returns the requested IJson object if it exists in the map file or in a related template file
* @param fieldName The name of the field to check
* @param main The main json file being parsed
* @param templ The template file json, if present, nullptr otherwise.
* @return the requested json object if found in the main json file, otherwise if it is found in the template and nullptr if not found anywhere
*/
    tson::IJson* tson::readField(const std::string& fieldName,  IJson& main, IJson* templ)
{
    if(main.count(fieldName) > 0) 
    {
        return &main[fieldName];
    } else if (templ && templ->count(fieldName) > 0)
    {
        return  &(*templ)[fieldName];
    }

    return nullptr;
}


/*!
* Attempts to read a text field from main file or the template if not overriden
* @param fieldName The name of the field to check
* @param main The main json file being parsed
* @param templ The template file json, if present, nullptr otherwise.
* @return true if the field was found and parsed in any of the objects, false otherwise
*/
bool tson::readField(Text& field, const std::string& fieldName,  IJson& main, IJson* templ) 
{
    IJson* fieldJson = readField(fieldName, main, templ);
    if(fieldJson){
        field = tson::Text(*fieldJson);
        return true;
    }
    return false;
}

/*!
* Attempts to read a series of coordinates from main file or the template if not overriden
* @param fieldName The name of the field to check
* @param main The main json file being parsed
* @param templ The template file json, if present, nullptr otherwise.
* @return true if the field was found and parsed in any of the objects, false otherwise
*/
bool tson::readField(std::vector<Vector2i>& field, const std::string& fieldName, IJson& main, IJson* templ)
{
    IJson* fieldJson = readField(fieldName, main, templ);
    if(fieldJson && fieldJson->isArray())
    {
        auto polyline = fieldJson->array();
        std::for_each(polyline.begin(), polyline.end(),[&field](std::unique_ptr<IJson> &item)
        {
            IJson &j = *item;
            field.emplace_back(j["x"].get<int>(), j["y"].get<int>());
        });
        return true;
    }
    return false;
}

/*!
* Attempts to read a vector from main file or the template if not overriden
* @param field Target variable to fill
* @param fieldNameX The name of the field to check for the x part of the vector
* @param fieldNameY The name of the field to check for the y part of the vector
* @param main The main json file being parsed
* @param templ The template file json, if present, nullptr otherwise.
* @return true if the field was found and parsed in any of the objects, false otherwise
*/
bool tson::readVector(Vector2i& field, const std::string& fieldNameX, const std::string& fieldNameY, IJson& main, IJson* templ) 
{
    int x = 0, y = 0;
    if(readField(x, fieldNameX, main, templ) && readField(y, fieldNameY, main, templ))
    {
        field = {x, y};
        return true;
    }   
    return false;
}

/*!
* Reads all custom properties from the given json node
* @param properties Target Properties collection to fill
* @param json json node representing the map object
* @param map Pointer to current map being parsed
*/
void tson::readProperties(tson::PropertyCollection& properties, IJson& json, tson::Map* map) 
{
    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &props = json.array("properties");
        tson::Project *project = (map != nullptr) ? map->getProject() : nullptr;
        std::for_each(props.begin(), props.end(), [&](std::unique_ptr<IJson> &item)
        {
            properties.add(*item, project);
        });
    }        
}

/*!
* Reads a gid, parsing flip-flags
* @param gid Target gid to fill
* @param flags Target flip flags to fill
* @param main The main json file being parsed
* @param templ The template file json, if present, nullptr otherwise.
*/
void tson::readGid(uint32_t& gid, TileFlipFlags& flags, IJson& main, IJson* templ)
{
    uint32_t g;
    if(readField(g, "gid", main, templ))
    {
        if (g & FLIPPED_HORIZONTALLY_FLAG) flags |= TileFlipFlags::Horizontally;
        if (g & FLIPPED_VERTICALLY_FLAG) flags |= TileFlipFlags::Vertically;
        if (g & FLIPPED_DIAGONALLY_FLAG) flags |= TileFlipFlags::Diagonally;
        
        // Clear flags
        g &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);
        
        gid = g;
    }
}


/*!
 * Parses a json Tiled object and automatically determines the object type based on the data presented.
 * Call getObjectType() to see what object type it is.
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Object::parse(IJson &json, tson::Map *map)
{
    m_map = map;
    bool allFound = true;
    IJson* templateJson = nullptr;
    
    if(readField(m_template, "template", json) && map != nullptr)
    {
        IJson* tobjJsonFile = map->parseLinkedFile(m_template);
        if(tobjJsonFile) 
            templateJson = readField("object", *tobjJsonFile);
    }

    readField(m_ellipse, "ellipse", json, templateJson); //Optional
    readField(m_point, "point", json, templateJson); // Optional
    readField(m_text, "text", json, templateJson);
    readGid(m_gid, m_flipFlags, json, templateJson);
    
    allFound &= readField(m_id, "id", json, templateJson);
    allFound &= readField(m_name, "name", json, templateJson);
    allFound &= readField(m_rotation, "rotation", json, templateJson);
    allFound &= readField(m_type, "type", json, templateJson) || readField(m_type, "class", json, templateJson); //Tiled v1.9 renamed 'type' to 'class'
    allFound &= readField(m_visible, "visible", json, templateJson);
    allFound &= readVector(m_size, "width", "height", json, templateJson);
    allFound &= readVector(m_position, "x", "y", json, templateJson);
    
    setObjectTypeByJson(json, templateJson); 
    
    if(m_objectType == ObjectType::Template)
        allFound = true; //Just accept anything with this type
    
    //More advanced data
    readField(m_polygon, "polygon", json, templateJson);
    readField(m_polyline, "polyline", json, templateJson);

    // merge properties with template's.
    if(templateJson) readProperties(m_properties, *templateJson, m_map);
    readProperties(m_properties, json, m_map);
    
    return allFound;
}

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
            m_class = std::make_shared<TiledClass>(*baseClass);//, m_map->getProject());
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

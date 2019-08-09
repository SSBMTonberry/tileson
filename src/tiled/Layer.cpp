//
// Created by robin on 01.08.2019.
//

#include "Layer.h"

/*!
 * Parses a Tiled layer from json
 * @param json
 */
tson::Layer::Layer(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Parses a Tiled layer from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Layer::parse(const nlohmann::json &json)
{
    bool allFound = true;
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
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("transparentcolor") > 0) m_transparentcolor = tson::Colori(json["transparentcolor"].get<std::string>()); //Optional
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); else allFound = false;
    if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;
    if(json.count("x") > 0) m_x = json["x"].get<int>(); else allFound = false;
    if(json.count("y") > 0) m_y = json["y"].get<int>(); else allFound = false;

    //Handle DATA (Optional)
    if(json.count("data") > 0)
    {
        if(json["data"].is_array())
        {
            std::for_each(json["data"].begin(), json["data"].end(), [&](const nlohmann::json &item) { m_data.push_back(item.get<int>()); });
        }
        else
            m_base64Data = json["data"].get<std::string>();
    }

    //More advanced data
    if(json.count("chunks") > 0 && json["chunks"].is_array())
        std::for_each(json["chunks"].begin(), json["chunks"].end(), [&](const nlohmann::json &item) { m_chunks.emplace_back(item); });
    if(json.count("layers") > 0 && json["layers"].is_array())
        std::for_each(json["layers"].begin(), json["layers"].end(), [&](const nlohmann::json &item) { m_layers.emplace_back(item); });
    if(json.count("objects") > 0 && json["objects"].is_array())
        std::for_each(json["objects"].begin(), json["objects"].end(), [&](const nlohmann::json &item) { m_objects.emplace_back(item); });
    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

/*!
 * 'compression': zlib, gzip or empty (default). tilelayer only.
 * @return
 */
const std::string &tson::Layer::getCompression() const
{
    return m_compression;
}

/*!
 * 'data' (when uint array): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::vector<int> &tson::Layer::getData() const
{
    return m_data;
}

/*!
 * 'data' (when string): Array of unsigned int (GIDs) or base64-encoded data. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getBase64Data() const
{
    return m_base64Data;
}

/*!
 * 'draworder': topdown (default) or index. objectgroup only.
 * @return
 */
const std::string &tson::Layer::getDrawOrder() const
{
    return m_drawOrder;
}

/*!
 * 'encoding': csv (default) or base64. tilelayer only.
 * @return
 */
const std::string &tson::Layer::getEncoding() const
{
    return m_encoding;
}

/*!
 * 'id': Incremental id - unique across all layers
 * @return
 */
int tson::Layer::getId() const
{
    return m_id;
}

/*!
 * 'image': Image used by this layer. imagelayer only.
 * @return
 */
const std::string &tson::Layer::getImage() const
{
    return m_image;
}

/*!
 * 'name': Name assigned to this layer
 * @return
 */
const std::string &tson::Layer::getName() const
{
    return m_name;
}

/*!
 * 'offsetx' and 'offsety': Horizontal and Vertical layer offset in pixels (default: {0, 0})
 * @return
 */
const tson::Vector2f &tson::Layer::getOffset() const
{
    return m_offset;
}

/*!
 * 'opacity': Value between 0 and 1
 * @return
 */
float tson::Layer::getOpacity() const
{
    return m_opacity;
}

/*!
 * x = 'width': (Column count. Same as map width for fixed-size maps.)
 * y = 'height': Row count. Same as map height for fixed-size maps.
 * @return width and height as a single size
 */
const tson::Vector2i &tson::Layer::getSize() const
{
    return m_size;
}

/*!
 * 'transparentcolor': Color created from a hex color (#RRGGBB) (optional, imagelayer only)
 * @return color as color object with rgba channel.
 */
const tson::Colori &tson::Layer::getTransparentcolor() const
{
    return m_transparentcolor;
}

/*!
 * 'type': tilelayer, objectgroup, imagelayer or group
 * @return string with the object type
 */
const std::string &tson::Layer::getType() const
{
    return m_type;
}

/*!
 * 'visible': Whether layer is shown or hidden in editor
 * @return
 */
bool tson::Layer::isVisible() const
{
    return m_visible;
}

/*!
 * 'x': Horizontal layer offset in tiles. Always 0.
 * @return x value (always 0 for layer)
 */
int tson::Layer::getX() const
{
    return m_x;
}

/*!
 * 'y': Horizontal layer offset in tiles. Always 0.
 * @return y value (always 0 for layer)
 */
int tson::Layer::getY() const
{
    return m_y;
}

/*!
 * 'chunks': Array of chunks (optional). tilelayer only.
 * @return
 */
std::vector<tson::Chunk> &tson::Layer::getChunks()
{
    return m_chunks;
}

/*!
 * 'layers': Array of layers. group on
 * @return
 */
std::vector<tson::Layer> &tson::Layer::getLayers()
{
    return m_layers;
}

/*!
 * 'objects': Array of objects. objectgroup only.
 * @return
 */
std::vector<tson::Object> &tson::Layer::getObjects()
{
    return m_objects;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Layer::getProperties()
{
    return m_properties;
}

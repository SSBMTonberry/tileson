//
// Created by robin on 02.08.2019.
//

#include "Object.h"

/*!
 * Parses a json Tiled object
 * @param json
 */
tson::Object::Object(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Parses a json Tiled object and autoamtically determines the object type based on the data presented.
 * Call getObjectType() to see what object type it is.
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Object::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("ellipse") > 0) m_ellipse = json["ellipse"].get<bool>(); //Optional
    if(json.count("gid") > 0) m_gid = json["gid"].get<int>(); //Optional
    if(json.count("id") > 0) m_id = json["id"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("point") > 0) m_point = json["point"].get<bool>(); //Optional
    if(json.count("rotation") > 0) m_rotation = json["rotation"].get<float>(); else allFound = false;
    if(json.count("template") > 0) m_template = json["template"].get<std::string>(); //Optional
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); else allFound = false;
    if(json.count("visible") > 0) m_visible = json["visible"].get<bool>(); else allFound = false;

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;
    if(json.count("x") > 0 && json.count("y") > 0)
        m_position = {json["x"].get<int>(), json["y"].get<int>()}; else allFound = false;

    if(json.count("text") > 0)
        m_text = {json["text"]["text"].get<std::string>(), json["text"]["wrap"].get<bool>()}; //Optional

    setObjectTypeByJson(json);

    if(m_objectType == Type::Template)
        allFound = true; //Just accept anything with this type

    //More advanced data
    if(json.count("polygon") > 0 && json["polygon"].is_array())
        std::for_each(json["polygon"].begin(), json["polygon"].end(),
                    [&](const nlohmann::json &item) { m_polygon.emplace_back(item["x"].get<int>(), item["y"].get<int>()); });

    if(json.count("polyline") > 0 && json["polyline"].is_array())
        std::for_each(json["polyline"].begin(), json["polyline"].end(),
                      [&](const nlohmann::json &item) { m_polyline.emplace_back(item["x"].get<int>(), item["y"].get<int>()); });

    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}


/*!
 * Sets an object type based on json data.
 * @param json
 */
void tson::Object::setObjectTypeByJson(const nlohmann::json &json)
{
    m_objectType = Type::Undefined;
    if(m_ellipse)
        m_objectType = Type::Ellipse;
    else if(m_point)
        m_objectType = Type::Point;
    else if(json.count("polygon") > 0)
        m_objectType = Type::Polygon;
    else if(json.count("polyline") > 0)
        m_objectType = Type::Polyline;
    else if(json.count("text") > 0)
        m_objectType = Type::Text;
    else if(json.count("gid") > 0)
        m_objectType = Type::Object;
    else if(json.count("template") > 0)
        m_objectType = Type::Template;
    else
        m_objectType = Type::Rectangle;
}

/*!
 * Gets what type of object this is.
 * @return
 */

tson::Object::Type tson::Object::getObjectType() const
{
    return m_objectType;
}

/*!
 * 'ellipse': Used to mark an object as an ellipse
 * @return
 */
bool tson::Object::isEllipse() const
{
    return m_ellipse;
}

/*!
 * 'gid': GID, only if object comes from a Tilemap
 * @return
 */
int tson::Object::getGid() const
{
    return m_gid;
}

/*!
 * x = 'width' (Width in pixels), y = 'height' (Height in pixels). Ignored if using a gid.)
 * @return
 */
const tson::Vector2i &tson::Object::getSize() const
{
    return m_size;
}

/*!
 * 'id': Incremental id - unique across all objects
 * @return
 */
int tson::Object::getId() const
{
    return m_id;
}

/*!
 * 'name': String assigned to name field in editor
 * @return
 */
const std::string &tson::Object::getName() const
{
    return m_name;
}

/*!
 * 'point': Used to mark an object as a point
 * @return true if the object is of type point
 */
bool tson::Object::isPoint() const
{
    return m_point;
}

/*!
 * 'rotation': Angle in degrees clockwise
 * @return
 */
float tson::Object::getRotation() const
{
    return m_rotation;
}

/*!
 * 'template': Reference to a template file, in case object is a template instance
 * @return
 */
const std::string &tson::Object::getTemplate() const
{
    return m_template;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const std::string &tson::Object::getType() const
{
    return m_type;
}

/*!
 * 'visible': Whether object is shown in editor.
 * @return
 */
bool tson::Object::isVisible() const
{
    return m_visible;
}

/*!
 * 'x' and 'y': coordinate in pixels
 * @return
 */
const tson::Vector2i &tson::Object::getPosition() const
{
    return m_position;
}

/*!
 * 'polygon': A list of x,y coordinates in pixels.
 * If this is a Polygon type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolygons() const
{
    return m_polygon;
}

/*!
 * 'polyline': A list of x,y coordinates in pixels
 * If this is a Polyline type, this function will return the points used to create it
 * @return
 */
const std::vector<tson::Vector2i> &tson::Object::getPolylines() const
{
    return m_polyline;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::Object::getProperties()
{
    return m_properties;
}

/*!
 * 'type': String assigned to type field in editor
 * @return
 */
const tson::Text &tson::Object::getText() const
{
    return m_text;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Object::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

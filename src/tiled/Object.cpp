//
// Created by robin on 02.08.2019.
//

#include "Object.h"

tson::Object::Object(const nlohmann::json &json)
{
    parse(json);
}

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

    setObjectTypeByJson(json);

    if(m_objectType == Type::Template)
        allFound = true; //Just accept anything with this type

    return allFound;
}

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

tson::Object::Type tson::Object::getObjectType() const
{
    return m_objectType;
}

bool tson::Object::isEllipse() const
{
    return m_ellipse;
}

int tson::Object::getGid() const
{
    return m_gid;
}

const tson::Vector2i &tson::Object::getSize() const
{
    return m_size;
}

int tson::Object::getId() const
{
    return m_id;
}

const std::string &tson::Object::getName() const
{
    return m_name;
}

bool tson::Object::isPoint() const
{
    return m_point;
}

float tson::Object::getRotation() const
{
    return m_rotation;
}

const std::string &tson::Object::getTemplate() const
{
    return m_template;
}

const std::string &tson::Object::getType() const
{
    return m_type;
}

bool tson::Object::isVisible() const
{
    return m_visible;
}

const tson::Vector2i &tson::Object::getPosition() const
{
    return m_position;
}

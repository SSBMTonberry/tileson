//
// Created by robin on 02.08.2019.
//

#include "Property.h"


tson::Property::Property() : m_name {"unnamed"}
{

}

tson::Property::Property(const nlohmann::json &json)
{
    setTypeByString(json["type"].get<std::string>());
    setValueByType(json["value"]);
    m_name = json["name"].get<std::string>();
}

tson::Property::Property(std::string name, std::any value, Type type) : m_name { move(name) }, m_value { move(value) }, m_type {type}
{

}

void tson::Property::setValue(const std::any &value)
{
    m_value = value;
}

/*!
 * Sets the value specifically as string.
 * When not specified as std::string, the default is that the value will be set as char * when adding a value like "test"
 * This function is to make sure the value is added as string.
 * @param value
 */
void tson::Property::setStrValue(const std::string &value)
{
    m_value = value;
}

const std::any &tson::Property::getValue() const
{
    return m_value;
}

void tson::Property::setName(const std::string &name)
{
    m_name = name;
}

const std::string &tson::Property::getName() const
{
    return m_name;
}

/*!
 * Gets the value type as std::value_info.
 * This can easily be compared to types like this:
 * Check if int: getValueType() == typeid(int)
 * @return
 */

const std::type_info &tson::Property::getValueType() const
{
    return m_value.type();
}

/*!
 * Gets the value type as std::string
 * Examples of known types:
 * "i" = int
 * "f" = float
 * "b" = bool
 * @return
 */
std::string tson::Property::getValueTypeInfo()
{
    return m_value.type().name();
}

tson::Property::Type tson::Property::getType() const
{
    return m_type;
}

void tson::Property::setTypeByString(const std::string &str)
{
    if(str == "color")
        m_type = tson::Property::Type::Color;
    else if(str == "file")
        m_type = tson::Property::Type::File;
    else if(str == "int")
        m_type = tson::Property::Type::Int;
    else if(str == "bool")
        m_type = tson::Property::Type::Boolean;
    else if(str == "float")
        m_type = tson::Property::Type::Float;
    else if(str == "string")
        m_type = tson::Property::Type::String;
    else
        m_type = tson::Property::Type::Undefined;
}

void tson::Property::setValueByType(const nlohmann::json &json)
{
    switch(m_type)
    {
        case Type::Color:
            m_value = Colori(json.get<std::string>());
            break;

        case Type::File:
            #if USE_CPP17_FILESYSTEM
                m_value = fs::path(json.get<std::string>());
            #else
                m_value = json.get<std::string>();
            #endif
            break;

        case Type::Int:
            m_value = json.get<int>();
            break;

        case Type::Boolean:
            m_value = json.get<bool>();
            break;

        case Type::Float:
            m_value = json.get<float>();
            break;

        case Type::String:
            setStrValue(json.get<std::string>());
            break;

        default:
            setStrValue(json.get<std::string>());
            break;

    }
}

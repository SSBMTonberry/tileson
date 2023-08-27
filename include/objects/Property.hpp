//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_PROPERTY_HPP
#define TILESON_PROPERTY_HPP

//#include "../../TilesonConfig.h"

//#if USE_CPP17_FILESYSTEM

#include <any>
#include <string>
#include "../common/Enums.hpp"
//#include "../external/json.hpp"
#include "Color.hpp"

namespace tson
{
    class Project;

    class Property
    {
        public:
            inline Property();
            inline explicit Property(IJson &json, tson::Project *project = nullptr);
            inline Property(std::string name, std::any value, Type type);

            inline void setValue(const std::any &value);
            inline void setStrValue(const std::string &value);
            inline void setName(const std::string &name);

            [[nodiscard]] inline const std::type_info& getValueType() const;
            inline std::string getValueTypeInfo();
            [[nodiscard]]inline const std::any &getValue() const;
            template <typename T>
            inline T getValue() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline Type getType() const;
            [[nodiscard]] inline const std::string &getPropertyType() const;

            //Became public in v1.4.0
            inline void setValueByType(IJson &json); //Definition in tileson_forward.hpp

        protected:
            inline void setTypeByString(const std::string &str);

            tson::Project *m_project = nullptr; //Used for resolving 'enum' and 'class' objects
            Type m_type = Type::Undefined;
            std::string m_name {};
            std::string m_propertyType {};
            std::any m_value; //Using std::any to assign any type
    };

    template<typename T>
    T Property::getValue() const
    {
        bool isCorrectType = (m_value.type() == typeid(T));

        if(isCorrectType)
        {
            T value = std::any_cast<T>(m_value);
            return value;
        }
        else
        {
            static T defaultValue;
            return defaultValue;
        }
    }
}

tson::Property::Property() : m_name {"unnamed"}
{

}

tson::Property::Property(IJson &json, tson::Project *project) : m_project {project}
{
    m_name = json["name"].get<std::string>();
    if(json.count("propertytype") > 0)
        m_propertyType = json["propertytype"].get<std::string>();
    else if(json.count("propertyType") > 0) //Somehow Tiled's class objects uses propertyType with 'T'.
        m_propertyType = json["propertyType"].get<std::string>();

    setTypeByString(json["type"].get<std::string>());
    setValueByType(json["value"]);
}

tson::Property::Property(std::string name, std::any value, Type type) : m_type {type}, m_name { std::move(name) }, m_value { std::move(value) }
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

tson::Type tson::Property::getType() const
{
    return m_type;
}

void tson::Property::setTypeByString(const std::string &str)
{
    if(str == "color")
        m_type = tson::Type::Color;
    else if(str == "file")
        m_type = tson::Type::File;
    else if(str == "int")
        m_type = tson::Type::Int;
    else if(str == "bool")
        m_type = tson::Type::Boolean;
    else if(str == "float")
        m_type = tson::Type::Float;
    else if(str == "string")
        m_type = tson::Type::String;
    else if(str == "class")
        m_type = tson::Type::Class;
    else if(str == "object")
        m_type = tson::Type::Object;
    else
        m_type = tson::Type::Undefined;
}

const std::string &tson::Property::getPropertyType() const
{
    return m_propertyType;
}

#endif //TILESON_PROPERTY_HPP

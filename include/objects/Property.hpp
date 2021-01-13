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
    class Property
    {
        public:

            //enum class Type : uint8_t
            //{
            //        Undefined = 0,
            //        Color = 1, /*! color */
            //        File = 2, /*! file */
            //        Int = 3, /*! int */
            //        Boolean = 4, /*! bool */
            //        Float = 5, /*! float */
            //        String = 6 /*! string */
            //};

            inline Property();
            inline Property(IJson &json);
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

        protected:
            inline void setTypeByString(const std::string &str);
            inline void setValueByType(IJson &json);

            Type m_type = Type::Undefined;
            std::string m_name;
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

tson::Property::Property(IJson &json)
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
    else
        m_type = tson::Type::Undefined;
}

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

#endif //TILESON_PROPERTY_HPP

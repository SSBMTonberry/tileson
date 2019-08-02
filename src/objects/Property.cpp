//
// Created by robin on 02.08.2019.
//

#include "Property.h"

tson::Property::Property() : m_name {"unnamed"}
{

}

tson::Property::Property(std::string name) : m_name { move(name) }
{

}

tson::Property::Property(std::string name, std::any value) : m_name { move(name) }, m_value { move(value) }
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
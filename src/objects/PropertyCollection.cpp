//
// Created by robin on 02.08.2019.
//

#include "PropertyCollection.h"

tson::PropertyCollection::PropertyCollection(std::string id) : m_id {std::move(id)}
{

}

tson::Property *tson::PropertyCollection::add(const tson::Property &property)
{
    m_properties[property.getName()] = property;
    return &m_properties[property.getName()];
}

tson::Property *tson::PropertyCollection::add(const nlohmann::json &json)
{
    tson::Property property = tson::Property(json);
    std::string name = property.getName();
    m_properties[name] = std::move(property);
    return &m_properties[name];
}

tson::Property *tson::PropertyCollection::add(const std::string &name, const std::any &value, tson::Property::Type type)
{
    m_properties[name] = {name, value, type};
    return &m_properties[name];
}

void tson::PropertyCollection::remove(const std::string &name)
{
    m_properties.erase(name);
}

/*!
 * Sets a value IF the property already exists. Does nothing otherwise.
 * See add() for adding new properties
 * @param name
 * @param value
 */
void tson::PropertyCollection::setValue(const std::string &name, const std::any &value)
{
    if(m_properties.count(name) > 0)
        m_properties[name].setValue(value);
}

void tson::PropertyCollection::setId(const std::string &id)
{
    m_id = id;
}

bool tson::PropertyCollection::hasProperty(const std::string &name)
{
    return m_properties.count(name) > 0;
}

tson::Property *tson::PropertyCollection::getProperty(const std::string &name)
{
    return (m_properties.count(name) > 0) ? &m_properties[name] : nullptr;
}

std::map<std::string, tson::Property> &tson::PropertyCollection::getProperties()
{
    return m_properties;
}

/*!
 * Gets vector of pointers to all the existing properties
 * @return
 */
std::vector<tson::Property *> tson::PropertyCollection::get()
{
    std::vector<tson::Property *> props;
    for(auto &i : m_properties)
        props.emplace_back(&i.second);

    return props;
}



const std::string &tson::PropertyCollection::getId() const
{
    return m_id;
}

size_t tson::PropertyCollection::getSize() const
{
    return m_properties.size();
}


//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_PROPERTYCOLLECTION_HPP
#define TILESON_PROPERTYCOLLECTION_HPP

#include "Property.hpp"
//#include "../external/json.hpp"
#include <map>

namespace tson
{
    class PropertyCollection
    {
        public:
            inline PropertyCollection() = default;

            inline explicit PropertyCollection(std::string id);

            inline tson::Property * add(const tson::Property &property);
            inline tson::Property * add(IJson &json);
            inline tson::Property * add(const std::string &name, const std::any &value, tson::Type type);

            inline void remove(const std::string &name);

            inline void setValue(const std::string &name, const std::any &value);
            inline void setId(const std::string &id);

            inline bool hasProperty(const std::string &name);
            inline tson::Property * getProperty(const std::string &name);
            inline std::map<std::string, Property> &getProperties();
            inline std::vector<Property*> get();
            template <typename T>
            inline T getValue(const std::string &name);
            [[nodiscard]] inline const std::string &getId() const;
            [[nodiscard]] inline size_t getSize() const;

        protected:
            std::string m_id;
            std::map<std::string, tson::Property> m_properties;
    };
}

template<typename T>
T tson::PropertyCollection::getValue(const std::string &name)
{
    static T defaultT;
    return (m_properties.count(name) > 0) ? m_properties[name].getValue<T>() : defaultT;
}

tson::PropertyCollection::PropertyCollection(std::string id) : m_id {std::move(id)}
{

}

tson::Property *tson::PropertyCollection::add(const tson::Property &property)
{
    m_properties[property.getName()] = property;
    return &m_properties[property.getName()];
}

tson::Property *tson::PropertyCollection::add(IJson &json)
{
    tson::Property property = tson::Property(json);
    std::string name = property.getName();
    m_properties[name] = std::move(property);
    return &m_properties[name];
}

tson::Property *tson::PropertyCollection::add(const std::string &name, const std::any &value, tson::Type type)
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

#endif //TILESON_PROPERTYCOLLECTION_HPP

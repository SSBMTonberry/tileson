//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TERRAIN_HPP
#define TILESON_TERRAIN_HPP

//#include "../external/json.hpp"
#include "../objects/PropertyCollection.hpp"

namespace tson
{
    class Terrain
    {
        public:
            inline Terrain() = default;
            inline Terrain(std::string name, int tile);
            inline explicit Terrain(IJson &json);

            inline bool parse(IJson &json);

            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline int getTile() const;
            [[nodiscard]] inline PropertyCollection &getProperties();

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

        private:
            std::string                 m_name;        /*! 'name': Name of terrain */
            int                         m_tile {};     /*! 'tile': Local ID of tile representing terrain */
            tson::PropertyCollection    m_properties;  /*! 'properties': A list of properties (name, value, type). */
    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::Terrain::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

tson::Terrain::Terrain(std::string name, int tile) : m_name {std::move(name)}, m_tile {tile}
{

}

tson::Terrain::Terrain(IJson &json)
{
    parse(json);
}

bool tson::Terrain::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item); });
    }

    return allFound;
}

/*!
 * 'name': Name of terrain
 * @return
 */
const std::string &tson::Terrain::getName() const
{
    return m_name;
}

/*!
 * 'tile': Local ID of tile representing terrain
 * @return
 */
int tson::Terrain::getTile() const
{
    return m_tile;
}

/*!
 * 'properties': A list of properties (name, value, type). *Missing from the official Tiled documentation...*
 * @return
 */
tson::PropertyCollection &tson::Terrain::getProperties()
{
    return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::Terrain::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);
    return nullptr;
}

#endif //TILESON_TERRAIN_HPP

//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGSET_HPP
#define TILESON_WANGSET_HPP

#include "../external/json.hpp"
#include "WangColor.hpp"
#include "WangTile.hpp"
#include "../objects/PropertyCollection.hpp"

namespace tson
{
    class WangSet
    {
        public:
            inline WangSet() = default;
            inline explicit WangSet(const nlohmann::json &json);
            inline bool parse(const nlohmann::json &json);

            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline int getTile() const;

            [[nodiscard]] inline const std::vector<tson::WangTile> &getWangTiles() const;
            [[nodiscard]] inline const std::vector<tson::WangColor> &getCornerColors() const;
            [[nodiscard]] inline const std::vector<tson::WangColor> &getEdgeColors() const;

            inline PropertyCollection &getProperties();

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

        private:
            std::string                  m_name;          /*! 'name': Name of the Wang set */
            int                          m_tile{};        /*! 'tile': Local ID of tile representing the Wang set */
            std::vector<tson::WangTile>  m_wangTiles;     /*! 'wangtiles': Array of Wang tiles */
            std::vector<tson::WangColor> m_cornerColors;  /*! 'cornercolors': Array of Wang colors */
            std::vector<tson::WangColor> m_edgeColors;    /*! 'edgecolors': Array of Wang colors */
            tson::PropertyCollection     m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

    };

    /*!
     * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
     * @tparam T The template value
     * @param name Name of the property
     * @return The actual value, if it exists. Otherwise: The default value of the type.
     */
    template<typename T>
    T tson::WangSet::get(const std::string &name)
    {
        return m_properties.getValue<T>(name);
    }
}

tson::WangSet::WangSet(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangSet::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;

    //More advanced data
    if(json.count("wangtiles") > 0 && json["wangtiles"].is_array())
        std::for_each(json["wangtiles"].begin(), json["wangtiles"].end(), [&](const nlohmann::json &item) { m_wangTiles.emplace_back(item); });
    if(json.count("cornercolors") > 0 && json["cornercolors"].is_array())
        std::for_each(json["cornercolors"].begin(), json["cornercolors"].end(), [&](const nlohmann::json &item) { m_cornerColors.emplace_back(item); });
    if(json.count("edgecolors") > 0 && json["edgecolors"].is_array())
        std::for_each(json["edgecolors"].begin(), json["edgecolors"].end(), [&](const nlohmann::json &item) { m_edgeColors.emplace_back(item); });
    if(json.count("properties") > 0 && json["properties"].is_array())
        std::for_each(json["properties"].begin(), json["properties"].end(), [&](const nlohmann::json &item) { m_properties.add(item); });

    return allFound;
}

/*!
 * 'name': Name of the Wang set
 * @return
 */
const std::string &tson::WangSet::getName() const
{
    return m_name;
}

/*!
 * 'tile': Local ID of tile representing the Wang set
 * @return
 */
int tson::WangSet::getTile() const
{
    return m_tile;
}

/*!
 * 'wangtiles': Array of Wang tiles
 * @return
 */
const std::vector<tson::WangTile> &tson::WangSet::getWangTiles() const
{
    return m_wangTiles;
}

/*!
 * 'cornercolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getCornerColors() const
{
    return m_cornerColors;
}

/*!
 * 'edgecolors': Array of Wang colors
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getEdgeColors() const
{
    return m_edgeColors;
}

/*!
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::WangSet::getProperties()
{
    return m_properties;
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::WangSet::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);

    return nullptr;
}

#endif //TILESON_WANGSET_HPP

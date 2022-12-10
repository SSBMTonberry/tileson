//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGSET_HPP
#define TILESON_WANGSET_HPP

//#include "../external/json.hpp"
#include "WangColor.hpp"
#include "WangTile.hpp"
#include "../objects/PropertyCollection.hpp"

namespace tson
{
    class WangSet
    {
        public:
            inline WangSet() = default;
            inline explicit WangSet(IJson &json, tson::Map *map);
            inline bool parse(IJson &json, tson::Map *map);

            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline int getTile() const;

            [[nodiscard]] inline const std::vector<tson::WangTile> &getWangTiles() const;
            [[nodiscard]] inline const std::vector<tson::WangColor> &getCornerColors() const;
            [[nodiscard]] inline const std::vector<tson::WangColor> &getEdgeColors() const;

            inline tson::WangColor * getColor(const std::string &name);
            inline const std::vector<tson::WangColor> &getColors() const;
            inline PropertyCollection &getProperties();

            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */

        private:

            inline bool parseTiled15Props(IJson &json);

            std::string                  m_name;          /*! 'name': Name of the Wang set */
            int                          m_tile{};        /*! 'tile': Local ID of tile representing the Wang set */
            std::vector<tson::WangTile>  m_wangTiles;     /*! 'wangtiles': Array of Wang tiles */
            std::vector<tson::WangColor> m_cornerColors;  /*! 'cornercolors': Array of Wang colors */
            std::vector<tson::WangColor> m_edgeColors;    /*! 'edgecolors': Array of Wang colors */
            tson::PropertyCollection     m_properties; 	  /*! 'properties': A list of properties (name, value, type). */

            //Tiled v1.5
            std::vector<tson::WangColor> m_colors;        /*! 'colors': */

            tson::Map *                  m_map;
            std::string                  m_classType {};              /*! 'class': The class of this map (since 1.9, defaults to “”). */
            std::shared_ptr<tson::TiledClass> m_class {};

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

tson::WangSet::WangSet(IJson &json, tson::Map *map)
{
    parse(json, map);
}

bool tson::WangSet::parse(IJson &json, tson::Map *map)
{
    m_map = map;
    bool allFound = true;

    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;

    //More advanced data
    if(json.count("wangtiles") > 0 && json["wangtiles"].isArray())
    {
        auto &wangtiles = json.array("wangtiles");
        std::for_each(wangtiles.begin(), wangtiles.end(), [&](std::unique_ptr<IJson> &item) { m_wangTiles.emplace_back(*item); });
    }
    if(json.count("cornercolors") > 0 && json["cornercolors"].isArray())
    {
        auto &cornercolors = json.array("cornercolors");
        std::for_each(cornercolors.begin(), cornercolors.end(), [&](std::unique_ptr<IJson> &item) { m_cornerColors.emplace_back(*item, m_map); });
    }
    if(json.count("edgecolors") > 0 && json["edgecolors"].isArray())
    {
        auto &edgecolors = json.array("edgecolors");
        std::for_each(edgecolors.begin(), edgecolors.end(), [&](std::unique_ptr<IJson> &item) { m_edgeColors.emplace_back(*item, m_map); });
    }
    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item); });
    }

    if(json.count("class") > 0) m_classType = json["class"].get<std::string>();                     //Optional

    if(!parseTiled15Props(json))
        allFound = false;

    return allFound;
}

/*!
 * Parsing data related to Tiled v1.5 changes
 * @param json The json containing data
 * @return Returns true if success
 */

bool tson::WangSet::parseTiled15Props(tson::IJson &json)
{
    if(json.count("colors") > 0 && json["colors"].isArray())
    {
        auto &colors = json.array("colors");
        std::for_each(colors.begin(), colors.end(), [&](std::unique_ptr<IJson> &item) { m_colors.emplace_back(*item, m_map); });
    }
    return true;
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

/*!
 * Get Wangset colors (new in Tiled v1.5)
 * @return
 */
const std::vector<tson::WangColor> &tson::WangSet::getColors() const
{
    return m_colors;
}

/*!
 * NB! Will only work with maps created/modified by Tiled v1.5 or later!
 * Gets a color from a wangset by its name.
 *
 * @param name 'name' of WangColor
 * @return The WangColor with the given name or nullptr if it doesn't exist.
 */
tson::WangColor *tson::WangSet::getColor(const std::string &name)
{
    auto color = std::find_if(m_colors.begin(), m_colors.end(), [&](const auto &c) { return c.getName() == name; });

    if(color != m_colors.end())
        return &color.operator*();

    return nullptr;
}

const std::string &tson::WangSet::getClassType() const
{
    return m_classType;
}




#endif //TILESON_WANGSET_HPP

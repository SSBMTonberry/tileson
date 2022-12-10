//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGCOLOR_HPP
#define TILESON_WANGCOLOR_HPP

//#include "../external/json.hpp"
#include "../objects/Color.hpp"

namespace tson
{
    class WangColor
    {
        public:
            inline WangColor() = default;
            inline explicit WangColor(IJson &json, tson::Map *map);
            inline bool parse(IJson &json, tson::Map *map);

            [[nodiscard]] inline const Colori &getColor() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline float getProbability() const;
            [[nodiscard]] inline int getTile() const;

            inline PropertyCollection &getProperties();
            template <typename T>
            inline T get(const std::string &name);
            inline tson::Property * getProp(const std::string &name);

            [[nodiscard]] inline const std::string &getClassType() const;
            [[nodiscard]] inline tson::TiledClass *getClass(); /*! Declared in tileson_forward.hpp */


        private:
            tson::Colori      m_color;              /*! 'color': Hex-formatted color (#RRGGBB or #AARRGGBB) */
            std::string       m_name;               /*! 'name': Name of the Wang color */
            float             m_probability{};      /*! 'probability': Probability used when randomizing */
            int               m_tile{};             /*! 'tile': Local ID of tile representing the Wang color */

            //New in Tiled v1.5
            tson::PropertyCollection     m_properties; 	  /*! 'properties': A list of properties (name, value, type). */
            tson::Map *                  m_map;
            std::string                  m_classType {};              /*! 'class': The class of this map (since 1.9, defaults to “”). */
            std::shared_ptr<tson::TiledClass> m_class {};

    };
}

tson::WangColor::WangColor(IJson &json, tson::Map *map)
{
    parse(json, map);
}

bool tson::WangColor::parse(IJson &json, tson::Map *map)
{
    m_map = map;
    bool allFound = true;

    if(json.count("color") > 0) m_color = tson::Colori(json["color"].get<std::string>()); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("probability") > 0) m_probability = json["probability"].get<float>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
    if(json.count("class") > 0) m_classType = json["class"].get<std::string>();                     //Optional

    if(json.count("properties") > 0 && json["properties"].isArray())
    {
        auto &properties = json.array("properties");
        std::for_each(properties.begin(), properties.end(), [&](std::unique_ptr<IJson> &item) { m_properties.add(*item); });
    }


    return allFound;
}

/*!
 * 'color': Color object created from hex-formatted string (#RRGGBB or #AARRGGBB)
 * @return
 */
const tson::Colori &tson::WangColor::getColor() const
{
    return m_color;
}

/*!
 * 'name': Name of the Wang color
 * @return
 */
const std::string &tson::WangColor::getName() const
{
    return m_name;
}

/*!
 * 'probability': Probability used when randomizing
 * @return
 */
float tson::WangColor::getProbability() const
{
    return m_probability;
}

/*!
 * 'tile': Local ID of tile representing the Wang color
 * @return
 */
int tson::WangColor::getTile() const
{
    return m_tile;
}

/*!
 * New property in Tiled v1.5 when data is contained in 'colors' of a wangset
 * 'properties': A list of properties (name, value, type).
 * @return
 */
tson::PropertyCollection &tson::WangColor::getProperties()
{
    return m_properties;
}

/*!
 * A shortcut for getting a property. Alternative to getProperties().getValue<T>("<name>")
 * @tparam T The template value
 * @param name Name of the property
 * @return The actual value, if it exists. Otherwise: The default value of the type.
 */
template<typename T>
T tson::WangColor::get(const std::string &name)
{
    return m_properties.getValue<T>(name);
}

/*!
 * Shortcut for getting a property object. Alternative to getProperties().getProperty("<name>");
 * @param name Name of the property
 * @return
 */
tson::Property *tson::WangColor::getProp(const std::string &name)
{
    if(m_properties.hasProperty(name))
        return m_properties.getProperty(name);

    return nullptr;
}

const std::string &tson::WangColor::getClassType() const
{
    return m_classType;
}

#endif //TILESON_WANGCOLOR_HPP

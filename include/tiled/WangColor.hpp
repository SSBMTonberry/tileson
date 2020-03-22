//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGCOLOR_HPP
#define TILESON_WANGCOLOR_HPP

#include "../external/json.hpp"
#include "../objects/Color.hpp"

namespace tson
{
    class WangColor
    {
        public:
            inline WangColor() = default;
            inline explicit WangColor(const nlohmann::json &json);
            inline bool parse(const nlohmann::json &json);

            [[nodiscard]] inline const Colori &getColor() const;
            [[nodiscard]] inline const std::string &getName() const;
            [[nodiscard]] inline float getProbability() const;
            [[nodiscard]] inline int getTile() const;

        private:
            tson::Colori      m_color;              /*! 'color': Hex-formatted color (#RRGGBB or #AARRGGBB) */
            std::string       m_name;               /*! 'name': Name of the Wang color */
            float             m_probability{};      /*! 'probability': Probability used when randomizing */
            int               m_tile{};             /*! 'tile': Local ID of tile representing the Wang color */
    };
}

tson::WangColor::WangColor(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangColor::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("color") > 0) m_color = tson::Colori(json["color"].get<std::string>()); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("probability") > 0) m_probability = json["probability"].get<float>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

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

#endif //TILESON_WANGCOLOR_HPP

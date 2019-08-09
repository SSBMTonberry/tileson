//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_WANGCOLOR_H
#define TILESON_WANGCOLOR_H

#include <json.hpp>
#include "../objects/Color.hpp"

namespace tson
{
    class WangColor
    {
        public:
            WangColor() = default;
            explicit WangColor(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] const Colori &getColor() const;
            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] float getProbability() const;
            [[nodiscard]] int getTile() const;

        private:
            tson::Colori      m_color;              /*! 'color': Hex-formatted color (#RRGGBB or #AARRGGBB) */
            std::string       m_name;               /*! 'name': Name of the Wang color */
            float             m_probability{};      /*! 'probability': Probability used when randomizing */
            int               m_tile{};             /*! 'tile': Local ID of tile representing the Wang color */
    };
}

#endif //TILESON_WANGCOLOR_H

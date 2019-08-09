//
// Created by robin on 05.08.2019.
//

#ifndef TILESON_GRID_H
#define TILESON_GRID_H

#include "../objects/Vector2.hpp"
#include <string>
#include <json.hpp>

namespace tson
{
    class Grid
    {
        public:
            Grid() = default;
            explicit Grid(const nlohmann::json &json);

            bool parse(const nlohmann::json &json);

            [[nodiscard]] const std::string &getOrientation() const;
            [[nodiscard]] const Vector2i &getSize() const;

        private:
            std::string m_orientation; /*! 'orientation': Orientation of the grid for the tiles in this tileset (orthogonal or isometric) */
            tson::Vector2i m_size; /*! 'width' and 'height': Size. */
    };
}

#endif //TILESON_GRID_H

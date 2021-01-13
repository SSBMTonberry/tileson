//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_GRID_HPP
#define TILESON_GRID_HPP

#include "../objects/Vector2.hpp"
#include <string>
//#include "../external/json.hpp"

namespace tson
{
    class Grid
    {
        public:
            inline Grid() = default;
            inline explicit Grid(IJson &json);

            inline bool parse(IJson &json);

            [[nodiscard]] inline const std::string &getOrientation() const;
            [[nodiscard]] inline const Vector2i &getSize() const;

        private:
            std::string m_orientation; /*! 'orientation': Orientation of the grid for the tiles in this tileset (orthogonal or isometric) */
            tson::Vector2i m_size; /*! 'width' and 'height': Size. */
    };
}

/*!
 * Parses Tiled grid data from json
 * @param json
 */
tson::Grid::Grid(IJson &json)
{
    parse(json);
}

/*!
 * Parses Tiled grid data from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Grid::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("orientation") > 0) m_orientation = json["orientation"].get<std::string>(); //Optional

    if(json.count("width") > 0 && json.count("height") > 0)
        m_size = {json["width"].get<int>(), json["height"].get<int>()}; else allFound = false;

    return allFound;
}

/*!
 * 'orientation': Orientation of the grid for the tiles in this tileset (orthogonal or isometric)
 * @return orientation as string
 */
const std::string &tson::Grid::getOrientation() const
{
    return m_orientation;
}

/*!
 * 'width' and 'height': Size.
 * @return size as int
 */
const tson::Vector2i &tson::Grid::getSize() const
{
    return m_size;
}

#endif //TILESON_GRID_HPP

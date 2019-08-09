//
// Created by robin on 05.08.2019.
//

#include "Grid.h"

/*!
 * Parses Tiled grid data from json
 * @param json
 */
tson::Grid::Grid(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Parses Tiled grid data from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Grid::parse(const nlohmann::json &json)
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

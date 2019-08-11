//
// Created by robin on 03.08.2019.
//

#include "WangTile.h"

tson::WangTile::WangTile(const nlohmann::json &json)
{
    parse(json);
}

/*!
 * Parses a wang tile from Tiled json.
 * @param json A Tiled json file
 * @return true if all mandatory fields were found. False otherwise.
 */
bool tson::WangTile::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("dflip") > 0) m_dflip = json["dflip"].get<bool>(); else allFound = false;
    if(json.count("hflip") > 0) m_hflip = json["hflip"].get<bool>(); else allFound = false;
    if(json.count("vflip") > 0) m_vflip = json["vflip"].get<bool>(); else allFound = false;
    if(json.count("tileid") > 0) m_tileid = json["tileid"].get<int>(); else allFound = false;

    if(json.count("wangid") > 0 && json["wangid"].is_array())
        std::for_each(json["wangid"].begin(), json["wangid"].end(), [&](const nlohmann::json &item) { m_wangId.emplace_back(item.get<int>()); });

    return allFound;
}

/*!
 * 'dflip': Tile is flipped diagonally
 * @return
 */
bool tson::WangTile::hasDFlip() const
{
    return m_dflip;
}

/*!
 * 'hflip': Tile is flipped horizontally
 * @return
 */
bool tson::WangTile::hasHFlip() const
{
    return m_hflip;
}

/*!
 * 'tileid': Local ID of tile
 * @return
 */
int tson::WangTile::getTileid() const
{
    return m_tileid;
}

/*!
 * 'vflip': Tile is flipped vertically
 * @return
 */
bool tson::WangTile::hasVFlip() const
{
    return m_vflip;
}

/*!
 * 'wangid': Array of Wang color indexes (uchar[8])
 * @return
 */
const std::vector<int> &tson::WangTile::getWangIds() const
{
    return m_wangId;
}

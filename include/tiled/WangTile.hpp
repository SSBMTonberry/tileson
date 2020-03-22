//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGTILE_HPP
#define TILESON_WANGTILE_HPP

#include "../external/json.hpp"

namespace tson
{
    class WangTile
    {
        public:
            inline WangTile() = default;
            inline explicit WangTile(const nlohmann::json &json);
            inline bool parse(const nlohmann::json &json);

            [[nodiscard]] inline bool hasDFlip() const;
            [[nodiscard]] inline bool hasHFlip() const;
            [[nodiscard]] inline int getTileid() const;
            [[nodiscard]] inline bool hasVFlip() const;

            [[nodiscard]] inline const std::vector<int> &getWangIds() const;

        private:
            bool                 m_dflip{};     /*! 'dflip': Tile is flipped diagonally */
            bool                 m_hflip{};     /*! 'hflip': Tile is flipped horizontally */
            int                  m_tileid{};    /*! 'tileid': Local ID of tile */
            bool                 m_vflip{};     /*! 'vflip': Tile is flipped vertically */
            std::vector<int>     m_wangId;      /*! 'wangid': Array of Wang color indexes (uchar[8])*/
    };
}

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

#endif //TILESON_WANGTILE_HPP

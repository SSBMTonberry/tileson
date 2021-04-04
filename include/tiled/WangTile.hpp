//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_WANGTILE_HPP
#define TILESON_WANGTILE_HPP

//#include "../external/json.hpp"

namespace tson
{
    class WangTile
    {
        public:
            inline WangTile() = default;
            inline explicit WangTile(IJson &json);
            inline bool parse(IJson &json);

            [[nodiscard]] inline bool hasDFlip() const;
            [[nodiscard]] inline bool hasHFlip() const;
            [[nodiscard]] inline uint32_t getTileid() const;
            [[nodiscard]] inline bool hasVFlip() const;

            [[nodiscard]] inline const std::vector<uint32_t> &getWangIds() const;

        private:
            bool                    m_dflip{};     /*! 'dflip': Tile is flipped diagonally */
            bool                    m_hflip{};     /*! 'hflip': Tile is flipped horizontally */
            uint32_t                m_tileid{};    /*! 'tileid': Local ID of tile */
            bool                    m_vflip{};     /*! 'vflip': Tile is flipped vertically */
            std::vector<uint32_t>   m_wangId;      /*! 'wangid': Array of Wang color indexes (uchar[8])*/
    };
}

tson::WangTile::WangTile(IJson &json)
{
    parse(json);
}

/*!
 * Parses a wang tile from Tiled json.
 * @param json A Tiled json file
 * @return true if all mandatory fields were found. False otherwise.
 */
bool tson::WangTile::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("dflip") > 0) m_dflip = json["dflip"].get<bool>(); //Removed in Tiled v1.5 and is now optional
    if(json.count("hflip") > 0) m_hflip = json["hflip"].get<bool>(); //Removed in Tiled v1.5 and is now optional
    if(json.count("vflip") > 0) m_vflip = json["vflip"].get<bool>(); //Removed in Tiled v1.5 and is now optional

    if(json.count("tileid") > 0) m_tileid = json["tileid"].get<uint32_t>(); else allFound = false;
    if(json.count("wangid") > 0 && json["wangid"].isArray())
    {
        auto &wangid = json.array("wangid");
        std::for_each(wangid.begin(), wangid.end(), [&](std::unique_ptr<IJson> &item) { m_wangId.emplace_back(item->get<uint32_t>()); });
    }

    return allFound;
}

/*!
 * 'dflip': Tile is flipped diagonally
 *
 * NB! This property got removed in Tiled v1.5
 * @return
 */
bool tson::WangTile::hasDFlip() const
{
    return m_dflip;
}

/*!
 * 'hflip': Tile is flipped horizontally
 *
 * NB! This property got removed in Tiled v1.5
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
uint32_t tson::WangTile::getTileid() const
{
    return m_tileid;
}

/*!
 * 'vflip': Tile is flipped vertically
 *
 * NB! This property got removed in Tiled v1.5
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
const std::vector<uint32_t> &tson::WangTile::getWangIds() const
{
    return m_wangId;
}

#endif //TILESON_WANGTILE_HPP

//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_WANGTILE_H
#define TILESON_WANGTILE_H

#include <json.hpp>

namespace tson
{
    class WangTile
    {
        public:
            WangTile() = default;
            explicit WangTile(const nlohmann::json &json);
            bool parse(const nlohmann::json &json);

            [[nodiscard]] bool hasDFlip() const;
            [[nodiscard]] bool hasHFlip() const;
            [[nodiscard]] int getTileid() const;
            [[nodiscard]] bool hasVFlip() const;

            [[nodiscard]] const std::vector<int> &getWangIds() const;

        private:
            bool                 m_dflip{};     /*! 'dflip': Tile is flipped diagonally */
            bool                 m_hflip{};     /*! 'hflip': Tile is flipped horizontally */
            int                  m_tileid{};    /*! 'tileid': Local ID of tile */
            bool                 m_vflip{};     /*! 'vflip': Tile is flipped vertically */
            std::vector<int>     m_wangId;      /*! 'wangid': Array of Wang color indexes (uchar[8])*/
    };
}

#endif //TILESON_WANGTILE_H

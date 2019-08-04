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

        private:
            bool        m_dflip{};     /*! 'dflip': */
            bool        m_hflip{};     /*! 'hflip': */
            int         m_tileid{};    /*! 'tileid': */
            bool        m_vflip{};     /*! 'vflip': */
            //TODO:     wangid 	array 	Array of Wang color indexes (uchar[8])

            /*
             * dflip 	bool 	Tile is flipped diagonally
               hflip 	bool 	Tile is flipped horizontally
               tileid 	int 	Local ID of tile
               vflip 	bool 	Tile is flipped vertically
               wangid 	array 	Array of Wang color indexes (uchar[8])
             * */
    };
}

#endif //TILESON_WANGTILE_H

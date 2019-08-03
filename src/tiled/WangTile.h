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

        private:
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

//
// Created by robin on 13.11.2020.
//

#ifndef TILESON_FLAGGEDTILE_HPP
#define TILESON_FLAGGEDTILE_HPP

namespace tson
{
    class FlaggedTile
    {

        public:
            FlaggedTile(size_t x_, size_t y_, uint32_t id_, uint32_t tileId_) : x {x_}, y {y_}, id {id_}, tileId {tileId_}
            {

            }
            size_t x;
            size_t y;
            /*! Full ID, including flag */
            uint32_t id;
            /*! ID of the flagged tile */
            uint32_t tileId;
    };
}
#endif //TILESON_FLAGGEDTILE_HPP

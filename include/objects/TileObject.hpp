//
// Created by robin on 26.07.2020.
//

#ifndef TILESON_TILEOBJECT_HPP
#define TILESON_TILEOBJECT_HPP

#include "../objects/Rect.hpp"

namespace tson
{
    class Tile;
    class TileObject
    {
        public:
            inline TileObject() = default;
            inline TileObject(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile);

            inline void initialize(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile); //Defined in tileson_forward.hpp

            inline Tile *getTile();
            inline const Vector2i &getPositionInTileUnits() const;
            inline const Vector2f &getPosition() const;
            inline const tson::Rect &getDrawingRect() const; //Defined in tileson_forward.hpp

        private:
            tson::Tile *m_tile;
            tson::Vector2i m_posInTileUnits;
            tson::Vector2f m_position;

    };

    TileObject::TileObject(const std::tuple<int, int> &posInTileUnits, tson::Tile *tile)
    {
        initialize(posInTileUnits, tile);
    }

    /*!
     * Get a pointer to the related tile
     * @return
     */
    Tile *TileObject::getTile()
    {
        return m_tile;
    }

    /*!
     * Gets the position of the tile in tile units
     * @return
     */
    const Vector2i &TileObject::getPositionInTileUnits() const
    {
        return m_posInTileUnits;
    }

    /*!
     * Gets the position of the tile in pixels.
     * @return
     */
    const Vector2f &TileObject::getPosition() const
    {
        return m_position;
    }
}

#endif //TILESON_TILEOBJECT_HPP

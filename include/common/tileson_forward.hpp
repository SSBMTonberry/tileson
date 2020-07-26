//
// Created by robin on 25.07.2020.
//

#ifndef TILESON_TILESON_FORWARD_HPP
#define TILESON_TILESON_FORWARD_HPP
/*!
 * T I L E S O N   F O R W A R D   D E C L A R A T I O N S
 * -------------------------------------------------------
 *
 * Due to cross-references we have forward declarations that cannot be resolved during the
 * implementation, thus the implementations must be done later when the class definition itself is known.
 *
 * All those forward declarations can be found below.
 */


// T i l e . h p p
// ---------------------

/*!
 * Really just a shortcut to retrieve the tile size from the map.
 * @return TileSize based on the map property for tile size.
 */
const tson::Vector2i tson::Tile::getTileSize() const
{
    if(m_map != nullptr)
        return m_map->getTileSize();
    else
        return {0,0};
}

/*!
 * Uses tson::Tileset and tson::Map data to calculate related values for tson::Tile.
 * Added in v1.2.0
 */
void tson::Tile::performDataCalculations()
{
    if(m_tileset == nullptr || m_map == nullptr)
        return;

    int firstId = m_tileset->getFirstgid(); //First tile id of the tileset
    int columns = m_tileset->getColumns();
    int rows = m_tileset->getTileCount() / columns;
    int lastId = (m_tileset->getFirstgid() + m_tileset->getTileCount()) - 1;

    if (getId() >= firstId && getId() <= lastId)
    {
        int baseTilePosition = (getId() - firstId);

        int tileModX = (baseTilePosition % columns);
        int currentRow = (baseTilePosition / columns);
        int offsetX = (tileModX != 0) ? ((tileModX) * m_map->getTileSize().x) : (0 * m_map->getTileSize().x);
        int offsetY =  (currentRow < rows-1) ? (currentRow * m_map->getTileSize().y) : ((rows-1) * m_map->getTileSize().y);

        m_drawingRect = { offsetX, offsetY, m_map->getTileSize().x, m_map->getTileSize().y };
    }
    else
        m_drawingRect = {0, 0, 0, 0};
}

/*!
 * Get the position of the tile in pixels based on the tile data position from the current layer.
 * @return The position of the tile in Pixels
 */
const tson::Vector2f tson::Tile::getPosition(const std::tuple<int, int> &tileDataPos)
{
    return {((float) std::get<0>(tileDataPos)) * m_drawingRect.width, ((float) std::get<1>(tileDataPos)) * m_drawingRect.height};
}

#endif //TILESON_TILESON_FORWARD_HPP

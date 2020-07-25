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
    int firstId = m_tileset->getFirstgid(); //First tile id of the tileset
    int columns = m_tileset->getColumns();
    int rows = m_tileset->getTileCount() / columns;
    int lastId = (m_tileset->getFirstgid() + m_tileset->getTileCount()) - 1;

    if (getId() >= firstId && getId() <= lastId)
    {
        //Get position in pixel units
        //tson::Vector2f position = {(float) std::get<0>(pos) * m_map->getTileSize().x, (float) std::get<1>(pos) * m_map->getTileSize().y};

        int baseTilePosition = (getId() - firstId);

        int tileModX = (baseTilePosition % columns);
        int currentRow = (baseTilePosition / columns);
        int offsetX = (tileModX != 0) ? ((tileModX) * m_map->getTileSize().x) : (0 * m_map->getTileSize().x);
        int offsetY =  (currentRow < rows-1) ? (currentRow * m_map->getTileSize().y) : ((rows-1) * m_map->getTileSize().y);

        m_drawingRect = { offsetX, offsetY, m_map->getTileSize().x, m_map->getTileSize().y };
        //Set sprite data to draw the tile
        //sf::Sprite *sprite = storeAndLoadImage(m_tileset->getImage().u8string(), {0,0});
        //if(sprite != nullptr)
        //{
        //    sprite->setTextureRect({offsetX, offsetY, m_map->getTileSize().x, m_map->getTileSize().y});
        //    sprite->setPosition({position.x, position.y});
        //    m_window.draw(*sprite);
        //}
    }
    else
        m_drawingRect = {0, 0, 0, 0};
}

/*!
 * Calculates position data based on incomming tile units
 * @param posInTileUnits position of the tile in tile units
 */
void tson::Tile::calculatePositionData(const tson::Vector2i &posInTileUnits)
{
    m_position = {(float) posInTileUnits.x * m_map->getTileSize().x, (float) posInTileUnits.y * m_map->getTileSize().y};
    m_tilePosition = posInTileUnits;
}


// L a y e r . h p p
// -------------------

/*!
 *
 * This is only supported for non-infinte maps!
 *
 * @param mapSize The size of the map
 * @param isInfiniteMap Whether or not the current map is infinte.
 */
void tson::Layer::createTileData(const Vector2i &mapSize, bool isInfiniteMap)
{
    size_t x = 0;
    size_t y = 0;
    if(!isInfiniteMap)
    {
        std::for_each(m_data.begin(), m_data.end(), [&](int tileId)
        {
            if (x == mapSize.x)
            {
                ++y;
                x = 0;
            }

            if (tileId > 0)
            {
                m_tileData[{x, y}] = m_tileMap[tileId];
                m_tileData[{x, y}]->calculatePositionData({(int)x, (int)y});
            }
            x++;
        });
    }
}

#endif //TILESON_TILESON_FORWARD_HPP

//
// Created by robin on 22.09.2019.
//

#include "SfmlDemoManager.h"

void SfmlDemoManager::initialize(const sf::Vector2i &windowSize, const sf::Vector2i &resolution, const std::string &title, const fs::path &basePath)
{
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, resolution.x, resolution.y)));
    m_basePath = basePath;
}

bool SfmlDemoManager::parseMap(const std::string &filename)
{
    tson::Tileson t;
    m_map = t.parse(fs::path(m_basePath / filename));


    fs::path demoTilesetPath = m_basePath / m_map.getTileset("demo-tileset")->getImage();

    if(m_map.getStatus() == tson::Map::ParseStatus::OK)
    {
        if(fs::exists(demoTilesetPath))
        {
            m_demoTilesetTexture.loadFromFile(demoTilesetPath.u8string());
            m_demoTilesetSprite.setTexture(m_demoTilesetTexture);

            return true;
        }
        else
            std::cout << "Invalid Demo Tileset Path: " << demoTilesetPath.u8string() << std::endl;
    }
    else
        std::cout << "Parse error: " << m_map.getStatusMessage() << std::endl;

    return false;
}

void SfmlDemoManager::drawMap()
{
    tson::Tileset* tileset = m_map.getTileset("demo-tileset"); /

    for(auto &layer : m_map.getLayers())
    {
        switch (layer.getType())
        {
            case tson::Layer::Type::TileLayer:
                drawTileLayer(layer, tileset);
                break;

            case tson::Layer::Type::ObjectGroup:
                //TODO: Implement
                break;

            case tson::Layer::Type::ImageLayer:
                //TODO: Implement
                break;

            case tson::Layer::Type::Group:
                //TODO: Implement
                break;

            default:
                break;
        }
    }
}

void SfmlDemoManager::run()
{
    while (m_window.isOpen())
    {
        // Process events
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            // Close m_window: exit
            if (event.type == sf::Event::Closed)
                m_window.close();
        }
        // Clear screen
        m_window.clear({35, 65, 90, 255});
        drawMap();
        m_window.display();
    }
}

void SfmlDemoManager::drawTileLayer(tson::Layer& layer, tson::Tileset* tileset)
{
    int firstId = tileset->getFirstgid(); //First tile id of the tileset
    int columns = tileset->getColumns(); //For the demo map it is 8.
    int rows = tileset->getTileCount() / columns;
    int lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

    //pos = position in tile units
    for (auto& [pos, tile] : layer.getTileData()) //Loops through absolutely all existing tiles
    {
        fs::path imagePath;
        std::string pathStr;
        //With this, I know that it's related to the tileset above (though I only have one tileset)
        if (tile->getId() >= firstId && tile->getId() <= lastId)
        {
            //Get position in pixel units
            tson::Vector2f position = {(float) std::get<0>(pos) * m_map.getTileSize().x, (float) std::get<1>(pos) * m_map.getTileSize().y};

            int baseTilePosition = (tile->getId() - firstId);

            int tileModX = (baseTilePosition % columns);
            int currentRow = (baseTilePosition / columns);
            int offsetX = (tileModX != 0) ? ((tileModX) * m_map.getTileSize().x) : (0 * m_map.getTileSize().x);
            int offsetY =  (currentRow < rows-1) ? (currentRow * m_map.getTileSize().y) : ((rows-1) * m_map.getTileSize().y);

            //Set sprite data to draw the tile
            m_demoTilesetSprite.setTextureRect({offsetX, offsetY, m_map.getTileSize().x, m_map.getTileSize().y});
            m_demoTilesetSprite.setPosition({position.x, position.y});
            m_window.draw(m_demoTilesetSprite);
        }
    }
}

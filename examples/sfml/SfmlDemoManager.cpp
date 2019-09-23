//
// Created by robin on 22.09.2019.
//

#include "SfmlDemoManager.h"

void SfmlDemoManager::initialize(const sf::Vector2i &windowSize, const sf::Vector2i &resolution, const std::string &title, const fs::path &basePath)
{
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float) resolution.x, (float)resolution.y)));
    m_basePath = basePath;
    m_font.loadFromMemory(vera_font::_VERA_TTF, vera_font::_VERA_TTF_SIZE);
}

bool SfmlDemoManager::parseMap(const std::string &filename)
{
    tson::Tileson t;
    m_map = t.parse(fs::path(m_basePath / filename));

    if(m_map.getStatus() == tson::Map::ParseStatus::OK)
    {
        for(auto &tileset : m_map.getTilesets())
            storeAndLoadImage(tileset.getImage().u8string(), {0,0});

        return true;
    }
    else
        std::cout << "Parse error: " << m_map.getStatusMessage() << std::endl;

    return false;
}

void SfmlDemoManager::drawMap()
{


    for(auto &layer : m_map.getLayers())
    {
        drawLayer(layer);
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
            sf::Sprite *sprite = storeAndLoadImage(tileset->getImage().u8string(), {0,0});
            if(sprite != nullptr)
            {
                sprite->setTextureRect({offsetX, offsetY, m_map.getTileSize().x, m_map.getTileSize().y});
                sprite->setPosition({position.x, position.y});
                m_window.draw(*sprite);
            }
        }
    }
}

void SfmlDemoManager::drawImageLayer(tson::Layer &layer)
{
    sf::Sprite *sprite = storeAndLoadImage(layer.getImage(), {layer.getOffset().x, layer.getOffset().y});
    if(sprite != nullptr)
        m_window.draw(*sprite);
}

void SfmlDemoManager::drawObjectLayer(tson::Layer &layer)
{
    tson::Tileset* tileset = m_map.getTileset("demo-tileset");

    for(auto &obj : layer.getObjects())
    {
        switch(obj.getObjectType())
        {
            case tson::Object::Type::Object:
            {
                sf::Vector2f offset = getTileOffset(obj.getGid());
                sf::Sprite *sprite = storeAndLoadImage(tileset->getImage().u8string(), {0,0});
                std::string name = obj.getName();
                if(sprite != nullptr)
                {
                    sprite->setTextureRect({(int)offset.x, (int)offset.y, m_map.getTileSize().x, m_map.getTileSize().y});
                    sprite->setPosition({(float)obj.getPosition().x, (float)obj.getPosition().y - m_map.getTileSize().y});
                    m_window.draw(*sprite);
                }
            }
            break;

            case tson::Object::Type::Ellipse:
                //Not used by the demo map, but you could use the properties of obj for a sf::CircleShape
                break;

            case tson::Object::Type::Rectangle:
                //Not used by the demo map, but you could use the properties of obj for a sf::RectangleShape
                break;

            case tson::Object::Type::Point:
                //Not used by the demo map but one could use the points of obj (polygon or polyline)
                //then pass them into logic like this:
                //sf::Vertex line[] =
                //        {
                //                sf::Vertex(sf::Vector2f(obj.getPolylines()[0].x, obj.getPolylines()[0].y)),
                //                sf::Vertex(sf::Vector2f(obj.getPolylines()[1].x, obj.getPolylines()[1].y))
                //        };
                //m_window.draw(line, 2, sf::Lines);
                break;

            case tson::Object::Type::Polygon:
                //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
                break;

            case tson::Object::Type::Polyline:
                //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
                break;

            case tson::Object::Type::Text:
                m_demoText.setFont(m_font);
                m_demoText.setPosition({(float)obj.getPosition().x, (float)obj.getPosition().y});
                m_demoText.setString(obj.getText().text);
                m_demoText.setCharacterSize(32); //It is 16, but makes it double for a "sharp text hack"
                m_demoText.setScale(0.5f, 0.5f); //Half scale for making a sharp text.
                m_window.draw(m_demoText);
                break;

            case tson::Object::Type::Template:
                //use obj.getTemplate() to get the connected template. References an external file not covered by Tileson.
                //obj.getPosition() and obj.getId() should also be related to the placement of the template.

                break;

            default:
                break;
        }
    }
}

/*!
 * Stores and loads the image if it doesn't exists, and retrieves it if it does.
 * @param image
 * @return
 */
sf::Sprite *SfmlDemoManager::storeAndLoadImage(const std::string &image, const sf::Vector2f &position)
{
    if(m_textures.count(image) == 0)
    {
        fs::path path = m_basePath / image;
        if(fs::exists(path))
        {
            std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
            tex->loadFromFile(path.u8string());
            std::unique_ptr<sf::Sprite> spr = std::make_unique<sf::Sprite>();
            spr->setTexture(*tex);
            spr->setPosition(position);
            m_textures[image] = std::move(tex);
            m_sprites[image] = std::move(spr);
        }
        else
            std::cout << "Could not find: " << path.u8string() << std::endl;
    }

    if(m_sprites.count(image) > 0)
        return m_sprites[image].get();

    return nullptr;
}

sf::Vector2f SfmlDemoManager::getTileOffset(int tileId)
{
    tson::Tileset* tileset = m_map.getTileset("demo-tileset");
    int firstId = tileset->getFirstgid(); //First tile id of the tileset
    int columns = tileset->getColumns(); //For the demo map it is 8.
    int rows = tileset->getTileCount() / columns;
    int lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

    //With this, I know that it's related to the tileset above (though I only have one tileset)
    if (tileId >= firstId && tileId <= lastId)
    {
        int baseTilePosition = (tileId - firstId);

        int tileModX = (baseTilePosition % columns);
        int currentRow = (baseTilePosition / columns);
        int offsetX = (tileModX != 0) ? ((tileModX) * m_map.getTileSize().x) : (0 * m_map.getTileSize().x);
        int offsetY =  (currentRow < rows-1) ? (currentRow * m_map.getTileSize().y) : ((rows-1) * m_map.getTileSize().y);
        return sf::Vector2f((float)offsetX, (float)offsetY);
    }

    return {0.f, 0.f};
}

void SfmlDemoManager::drawLayer(tson::Layer &layer)
{
    tson::Tileset* tileset = m_map.getTileset("demo-tileset");
    switch (layer.getType())
    {
        case tson::Layer::Type::TileLayer:
            drawTileLayer(layer, tileset);
            break;

        case tson::Layer::Type::ObjectGroup:
            drawObjectLayer(layer);
            break;

        case tson::Layer::Type::ImageLayer:
            drawImageLayer(layer);
            break;

        case tson::Layer::Type::Group:
            //There are no group layers in the demo map, but it basicly just contains sub layers
            //You can call this function on those layers, like this:
            for(auto &l : layer.getLayers())
                drawLayer(l);

            break;

        default:
            break;
    }
}

//
// Created by robin on 22.09.2019.
//

#include "SfmlDemoManager.h"

void SfmlDemoManager::initialize(const sf::Vector2i &windowSize, const sf::Vector2i &resolution, const std::string &title, const fs::path &basePath)
{
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float) resolution.x, (float)resolution.y)));
    m_basePath = basePath;
    #if __clang__
    fs::path appRoot = getMacApplicationFolder(true);
    m_basePath = appRoot / m_basePath;
    #endif
    m_font.loadFromMemory(vera_font::_VERA_TTF, vera_font::_VERA_TTF_SIZE);
    ImGui::SFML::Init(m_window);
}

bool SfmlDemoManager::parseMap(const std::string &filename)
{
    tson::Tileson t;
    m_map = t.parse(fs::path(m_basePath / filename));

    if(m_map->getStatus() == tson::ParseStatus::OK)
    {
        for(auto &tileset : m_map->getTilesets())
        {
            fs::path tilesetPath = getTilesetImagePath(tileset); //tileset.getImage().u8string()
            storeAndLoadImage(tilesetPath, {0, 0});
        }

        return true;
    }
    else
        std::cout << "Parse error: " << m_map->getStatusMessage() << std::endl;

    return false;
}

bool SfmlDemoManager::parseProject(const std::string &filename)
{
    tson::Tileson t;
    fs::path projectBase = fs::path(m_basePath / "project");
    int projectCount {0};
    int worldCount {0};
    if(m_project.parse(fs::path(projectBase / filename)))
    {
        for(const auto &folder : m_project.getFolders())
        {
            if(folder.hasWorldFile())
            {
                const auto &world = folder.getWorld();
                for(const auto &data : world.getMapData())
                {
                    std::unique_ptr<tson::Map> map = t.parse(fs::path(world.getFolder() / data.fileName));
                    if(map->getStatus() == tson::ParseStatus::OK)
                    {
                        ++worldCount;
                        for(auto &tileset : map->getTilesets())
                        {
                            fs::path tilesetPath = getTilesetImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
                            storeAndLoadImage(tilesetPath, {0, 0});
                        }

                        m_worldMaps.push_back(std::move(map));
                        m_worldData.emplace_back(data);
                    }
                    else
                    {
                        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
                        return false;
                    }
                }
            }
            else
            {
                for(const auto &file : folder.getFiles())
                {
                    std::unique_ptr<tson::Map> map = t.parse(fs::path(folder.getPath() / file.filename()));
                    if(map->getStatus() == tson::ParseStatus::OK)
                    {
                        ++projectCount;

                        for(auto &tileset : map->getTilesets())
                        {
                            fs::path tilesetPath = getTilesetImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
                            storeAndLoadImage(tilesetPath, {0, 0});
                        }
                        m_projectMaps.push_back(std::move(map));

                    }
                    else
                    {
                        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        std::cout << "Project parse error! " << std::endl;
        return false;
    }

    return true;
}

void SfmlDemoManager::drawMap()
{

    if(m_mapIndex == 0) m_currentMap = m_map.get();
    else if(m_mapIndex == 1) m_currentMap = m_projectMaps.at(1).get();
    else if(m_mapIndex == 2) m_currentMap = m_projectMaps.at(0).get();
    else if(m_mapIndex == 3) m_currentMap = m_projectMaps.at(2).get();

    if(m_mapIndex < 4)
    {
        if (m_currentMap != nullptr)
        {
            for (auto &layer : m_currentMap->getLayers())
                drawLayer(layer);
        }
    }
    else //WORLD
    {
        for(int i = 0; i < m_worldMaps.size(); ++i)
        {
            m_currentMap = m_worldMaps.at(i).get();
            const tson::WorldMapData &data = m_worldData.at(i);
            for (auto &layer : m_currentMap->getLayers())
                drawLayer(layer);
        }
    }

    //for(auto &layer : m_map->getLayers())
    //{
    //    drawLayer(layer);
    //}
}

void SfmlDemoManager::drawImgui()
{

    ImGui::Begin("Maps");
    std::string mapsStr = std::to_string(m_mapIndex) + " of " + std::to_string(m_maxMapIndex);
    ImGui::PushItemWidth(45);
    ImGui::LabelText(mapsStr.c_str(), "Map: ");
    //ImGui::PopItemWidth();
    //ImGui::SameLine();
    //ImGui::LabelText("###second", );

    //World related data
    if(m_mapIndex > 3 && m_currentMap != nullptr)
    {
        //RBP: Add info here
    }
    if(ImGui::Button("<<"))
    {
        --m_mapIndex;
        if(m_mapIndex < 0)
            m_mapIndex = m_maxMapIndex;
    }
    ImGui::SameLine();
    if(ImGui::Button(">>"))
    {
        ++m_mapIndex;
        if(m_mapIndex > m_maxMapIndex)
            m_mapIndex = 0;
    }
    ImGui::End();
}

void SfmlDemoManager::run()
{
    sf::Clock deltaClock;
    while (m_window.isOpen())
    {
        // Process events
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            // Close m_window: exit
            if (event.type == sf::Event::Closed)
                m_window.close();
        }
        ImGui::SFML::Update(m_window, deltaClock.restart());

        // Clear screen
        m_window.clear({35, 65, 90, 255});
        drawMap();
        drawImgui();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void SfmlDemoManager::drawTileLayer(const tson::Layer& layer)//, tson::Tileset* tileset)
{
    //pos = position in tile units
    for (const auto& [pos, tileObject] : layer.getTileObjects()) //Loops through absolutely all existing tiles
    {
        //Set sprite data to draw the tile
        tson::Tileset *tileset = tileObject.getTile()->getTileset();
        tson::Rect drawingRect = tileObject.getDrawingRect();
        tson::Vector2f position = tileObject.getPosition();

        sf::Sprite *sprite = storeAndLoadImage(getTilesetImagePath(*tileset), {0, 0});
        if (sprite != nullptr)
        {
            sf::Vector2f scale = sprite->getScale();
            sf::Vector2f originalScale = scale;
            float rotation = sprite->getRotation();
            float originalRotation = rotation;
            sf::Vector2f origin {((float)drawingRect.width) / 2, ((float)drawingRect.height) / 2};

            if(tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Horizontally))
                scale.x = -scale.x;
            if(tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Vertically))
                scale.y = -scale.y;
            if(tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Diagonally))
                rotation += 90.f;

            position = {position.x + origin.x, position.y + origin.y};
            sprite->setOrigin(origin);
            sprite->setTextureRect({drawingRect.x, drawingRect.y, drawingRect.width, drawingRect.height});
            sprite->setPosition({position.x, position.y});

            sprite->setScale(scale);
            sprite->setRotation(rotation);

            m_window.draw(*sprite);

            sprite->setScale(originalScale);       //Since we used a shared sprite for this example, we must reset the scale.
            sprite->setRotation(originalRotation); //Since we used a shared sprite for this example, we must reset the rotation.
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
    //tson::Tileset* tileset = m_map->getTileset("demo-tileset");
    auto *map = layer.getMap();
    for(auto &obj : layer.getObjects())
    {
        switch(obj.getObjectType())
        {
            case tson::ObjectType::Object:
            {
                tson::Tileset *tileset = layer.getMap()->getTilesetByGid(obj.getGid());
                sf::Vector2f offset = getTileOffset(obj.getGid(), map, tileset);

                sf::Sprite *sprite = storeAndLoadImage(getTilesetImagePath(*tileset), {0,0});
                std::string name = obj.getName();
                sf::Vector2f position = {(float)obj.getPosition().x, (float)obj.getPosition().y};
                if(sprite != nullptr)
                {
                    sf::Vector2f scale = sprite->getScale();
                    sf::Vector2f originalScale = scale;
                    float rotation = sprite->getRotation();
                    float originalRotation = rotation;
                    sf::Vector2f origin {((float)m_map->getTileSize().x) / 2, ((float)map->getTileSize().y) / 2};

                    if(obj.hasFlipFlags(tson::TileFlipFlags::Horizontally))
                        scale.x = -scale.x;
                    if(obj.hasFlipFlags(tson::TileFlipFlags::Vertically))
                        scale.y = -scale.y;
                    if(obj.hasFlipFlags(tson::TileFlipFlags::Diagonally))
                        rotation += 90.f;

                    position = {position.x + origin.x, position.y + origin.y};
                    sprite->setOrigin(origin);

                    sprite->setTextureRect({(int)offset.x, (int)offset.y, map->getTileSize().x, map->getTileSize().y});
                    sprite->setPosition({position.x, position.y - map->getTileSize().y});

                    sprite->setScale(scale);
                    sprite->setRotation(rotation);

                    m_window.draw(*sprite);

                    sprite->setScale(originalScale);       //Since we used a shared sprite for this example, we must reset the scale.
                    sprite->setRotation(originalRotation); //Since we used a shared sprite for this example, we must reset the rotation.
                }
            }
            break;

            case tson::ObjectType::Ellipse:
                //Not used by the demo map, but you could use the properties of obj for a sf::CircleShape
                break;

            case tson::ObjectType::Rectangle:
                //Not used by the demo map, but you could use the properties of obj for a sf::RectangleShape
                break;

            case tson::ObjectType::Point:
                //Not used by the demo map but one could use the points of obj (polygon or polyline)
                //then pass them into logic like this:
                //sf::Vertex line[] =
                //        {
                //                sf::Vertex(sf::Vector2f(obj.getPolylines()[0].x, obj.getPolylines()[0].y)),
                //                sf::Vertex(sf::Vector2f(obj.getPolylines()[1].x, obj.getPolylines()[1].y))
                //        };
                //m_window.draw(line, 2, sf::Lines);
                break;

            case tson::ObjectType::Polygon:
                //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
                break;

            case tson::ObjectType::Polyline:
                //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
                break;

            case tson::ObjectType::Text:
                m_demoText.setFont(m_font);
                m_demoText.setPosition({(float)obj.getPosition().x, (float)obj.getPosition().y});
                m_demoText.setString(obj.getText().text);
                m_demoText.setCharacterSize(32); //It is 16, but makes it double for a "sharp text hack"
                m_demoText.setScale(0.5f, 0.5f); //Half scale for making a sharp text.
                m_window.draw(m_demoText);
                break;

            case tson::ObjectType::Template:
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

sf::Vector2f SfmlDemoManager::getTileOffset(int tileId, tson::Map *map, tson::Tileset* tileset)
{

    //tson::Tileset* tileset = map->getTileset("demo-tileset");

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
        int offsetX = (tileModX != 0) ? ((tileModX) * map->getTileSize().x) : (0 * map->getTileSize().x);
        int offsetY = (currentRow < rows - 1) ? (currentRow * map->getTileSize().y) : ((rows - 1) * map->getTileSize().y);
        return sf::Vector2f((float) offsetX, (float) offsetY);
    }

    return {0.f, 0.f};
}

void SfmlDemoManager::drawLayer(tson::Layer &layer)
{

    switch (layer.getType())
    {
        case tson::LayerType::TileLayer:
            drawTileLayer(layer);
            break;

        case tson::LayerType::ObjectGroup:
            drawObjectLayer(layer);
            break;

        case tson::LayerType::ImageLayer:
            drawImageLayer(layer);
            break;

        case tson::LayerType::Group:
            //There are no group layers in the demo map, but it basicly just contains sub layers
            //You can call this function on those layers, like this:
            for(auto &l : layer.getLayers())
                drawLayer(l);

            break;

        default:
            break;
    }
}

/*!
 * Just a helper function to get the common path to the actual image files
 * @param tileset
 * @return
 */
fs::path SfmlDemoManager::getTilesetImagePath(const tson::Tileset &tileset)
{
    fs::path path = fs::path(fs::path("../") / tileset.getImage().filename().u8string());
    return path;
}



#if __clang__
fs::path SfmlDemoManager::getMacApplicationFolder(bool isAppPath)
{
    char buf [PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if(!_NSGetExecutablePath(buf, &bufsize))
        puts(buf);

    fs::path path {buf};
    //Using parent_path several times to get to the part of the .app file where we are allowed to
    //produce a file. It is still inside the .app-file, which makes it possible to move preferences
    //with the file itself.
    if(isAppPath)
        return path.parent_path().parent_path().parent_path().parent_path();
    else
        return path.parent_path().parent_path();
}
#endif
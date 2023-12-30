//
// Created by robin on 22.09.2019.
//

#include "SfmlDemoManager.h"

bool SfmlDemoManager::initialize(const sf::Vector2i &windowSize, const sf::Vector2i &resolution, const std::string &title, const fs::path &basePath)
{
    m_window.create(sf::VideoMode(windowSize.x, windowSize.y), title, sf::Style::Titlebar | sf::Style::Close);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float) resolution.x, (float)resolution.y)));
    m_window.setFramerateLimit(60);

    m_basePath = basePath;
    #if __clang__
    fs::path appRoot = getMacApplicationFolder(true);
    m_basePath = appRoot / m_basePath;
    #endif
    m_font.loadFromMemory(vera_font::_VERA_TTF, vera_font::_VERA_TTF_SIZE);
    ImGui::SFML::Init(m_window);
    ImGui::GetIO().IniFilename = nullptr;

    //Initialize maps
    m_map = parseMap("ultimate_test.json");
    m_imageCollectionTilesetMap = parseMap("feature_111_demo_map.json");
    m_marginSpaceMap = parseMap("margin-space-map.lzma", std::make_unique<tson::Lzma>());
    bool projectOk = parseProject();

    return (m_map != nullptr && m_imageCollectionTilesetMap != nullptr && m_marginSpaceMap != nullptr && projectOk);
}

std::unique_ptr<tson::Map> SfmlDemoManager::parseMap(const std::string &filename, std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor)
{
    tson::Tileson t;
    std::unique_ptr<tson::Map> map = t.parse(fs::path(m_basePath / filename), std::move(decompressor));

    if(map->getStatus() == tson::ParseStatus::OK)
    {
        for(auto &tileset : map->getTilesets())
        {            
            if (tileset.getType() == tson::TilesetType::ImageTileset)
            {
                fs::path tilesetPath = getImagePath(tileset); //tileset.getImage().u8string()

            }
            else if (tileset.getType() == tson::TilesetType::ImageCollectionTileset)
            {
                // Image is specific to each tile
                for (const tson::Tile& tile : tileset.getTiles())
                {
                    fs::path tilesetPath = getImagePath(tile); //tile.getImage().u8string()
                    storeAndLoadImage(tilesetPath.string(), { 0, 0 });
                }
            }
        }

        return std::move(map);
    }
    else
        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;

    return nullptr;
}

bool SfmlDemoManager::parseProject(const std::string &filename)
{
    using namespace std::string_literals; //Used for s-suffix
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
                            fs::path tilesetPath = getImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
                            storeAndLoadImage(tilesetPath.string(), {0, 0});
                        }

                        m_worldMaps.push_back(std::move(map));
                        m_worldData.emplace_back(data);
                        m_worldVisibilityFlags.push_back(true);
                        std::string info = "Part of .world file (inside a project) '"s + world.getPath().filename().string() + "': " + data.fileName;
                        m_worldMapInfo.emplace_back(info);
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
                            fs::path tilesetPath = getImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
                            storeAndLoadImage(tilesetPath.string(), {0, 0});
                        }
                        m_projectMaps[file.filename().string()] = std::move(map);
                        std::string info = "Part of project file '"s + m_project.getPath().filename().string() + "': " + file.filename().string();
                        m_projectMapInfo.emplace_back(info);
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
    m_positionOffset = {0,0};
    m_currentInfo = (m_mapIndex == 0) ? "This is just a regular Tiled json-map" : "";
    if(m_mapIndex == 0) m_currentMap = m_map.get();
    else if (m_mapIndex == 1)
    {
        m_currentMap = m_imageCollectionTilesetMap.get();
        m_currentInfo = "Image Collection Tileset demo";
    }
    else if(m_mapIndex == 2)
    {
        m_currentMap = m_projectMaps["map1.json"].get(); //Lazy way of getting the map by index
        m_currentInfo = m_projectMapInfo.at(0);
    }
    else if(m_mapIndex == 3)
    {
        m_currentMap = m_projectMaps["map2.json"].get(); //Lazy way of getting the map by index
        m_currentInfo = m_projectMapInfo.at(1);
    }
    else if(m_mapIndex == 4)
    {
        m_currentMap = m_projectMaps["map3.json"].get();
        m_currentInfo = m_projectMapInfo.at(2);
    }
    else if(m_mapIndex == 5)
    {
        m_currentMap = m_marginSpaceMap.get();
        m_currentInfo = "This map uses margin and spacing in tileset and is also LZMA-compressed!";
    }

    if(m_mapIndex < 6)
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
            tson::Vector2i tileSize = m_currentMap->getTileSize();

            const tson::WorldMapData &data = m_worldData.at(i);
            m_positionOffset = {(data.position.x + data.size.x), (data.position.y + data.size.y)};
            bool isVisible = m_worldVisibilityFlags[i];
            if(isVisible)
            {
                for (auto &layer : m_currentMap->getLayers())
                    drawLayer(layer);
            }

        }
    }

    //for(auto &layer : m_map->getLayers())
    //{
    //    drawLayer(layer);
    //}
}

void SfmlDemoManager::drawImgui()
{

    if(!m_isImguiSizeSet)
    {
        ImGui::SetNextWindowSize({300, 400});
        m_isImguiSizeSet = true;
    }
    ImGui::Begin("Maps");
    float timeDeltaMs = (float)((double)m_timeDelta.asMicroseconds() / 1000);
    float fps = 1000.f / timeDeltaMs;
    ImGui::TextWrapped("FPS: %.1f (Time delta: %.3f ms)", fps, timeDeltaMs);

    std::string mapsStr = std::to_string(m_mapIndex) + " of " + std::to_string(m_maxMapIndex);
    ImGui::PushItemWidth(45);
    ImGui::LabelText(mapsStr.c_str(), "Map: ");

    if(ImGui::Button("<-"))
    {
        --m_mapIndex;
        if(m_mapIndex < 0)
            m_mapIndex = m_maxMapIndex;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Previous");
    ImGui::SameLine();

    if(ImGui::Button("->"))
    {
        ++m_mapIndex;
        if(m_mapIndex > m_maxMapIndex)
            m_mapIndex = 0;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Next");

    //World related data
    if(m_mapIndex > 5 && m_currentMap != nullptr)
    {
        //RBP: Add info here
        for(int i = 0; i < m_worldVisibilityFlags.size(); ++i)
        {
            std::string id = "Show world - part " + std::to_string(i+1) + "###visibility" + std::to_string(i);
            bool checked = m_worldVisibilityFlags[i];
            ImGui::TextWrapped("%s", m_worldMapInfo.at(i).c_str());
            if(ImGui::Checkbox(id.c_str(), &checked))
                m_worldVisibilityFlags[i] = checked;
        }
    }
    else
    {
        ImGui::TextWrapped("%s", m_currentInfo.c_str());
    }


    if(m_mapIndex == 0)
    {
        ImGui::Text("Animation data:");
        for(auto &[id, animation] : m_animationUpdateQueue)
        {
            ImGui::TextWrapped("Frame %d (duration %d of %d) - tile: %d", animation->getCurrentFrameNumber(), (int)animation->getTimeDelta(),
                        animation->getCurrentFrame()->getDuration(), animation->getCurrentTileId());
        }
    }

    ImGui::End();
}

void SfmlDemoManager::updateAnimations()
{
    for(auto &[id, animation] : m_animationUpdateQueue)
    {
        //Time needs to be received as microseconds to get the right precision.
        float ms = (float)((double)m_timeDelta.asMicroseconds() / 1000);
        animation->update(ms);
    }
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
        m_timeDelta = deltaClock.restart();
        updateAnimations();
        ImGui::SFML::Update(m_window, m_timeDelta);
        // Clear screen
        m_window.clear({35, 65, 90, 255});
        drawMap();
        drawImgui();
        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void SfmlDemoManager::drawTexture(const sf::Texture& texture, const sf::IntRect& textureRegion, const sf::Vector2f& position,
    int32_t tileHeight, bool hortFlip, bool vertFlip, bool diagonalFlip)
{
    sf::Sprite sprite(texture, textureRegion);

    // Center origin so scale and rotation does not displace sprite
    sf::Vector2f halfSize = sf::Vector2f(textureRegion.width / 2.0f, textureRegion.height / 2.0f);
    sprite.setOrigin(halfSize);
    sf::Vector2f originOffset{ halfSize.x, halfSize.y + tileHeight - textureRegion.height };

    if (diagonalFlip)
    {
        sprite.rotate(90);

        bool temp = hortFlip;
        hortFlip = vertFlip;
        vertFlip = !hortFlip;

        // Compensate for the swap of image dimensions
        float halfDiff = halfSize.y - halfSize.x;
        originOffset.y += halfDiff;
        originOffset.x += halfDiff;
    }

    // Scale
    float scaleX = hortFlip ? -1.0f : 1.0f;
    float scaleY = vertFlip ? -1.0f : 1.0f;
    sprite.scale(scaleX, scaleY);

    // Position
    sprite.setPosition(originOffset.x + position.x, originOffset.y + position.y);

    m_window.draw(sprite);
}

void SfmlDemoManager::drawTileLayer(tson::Layer& layer)//, tson::Tileset* tileset)
{
    float tileHeight = layer.getMap()->getTileSize().y;

    //pos = position in tile units
    for (auto& [pos, tileObject] : layer.getTileObjects()) //Loops through absolutely all existing tiles
    {
        //Set sprite data to draw the tile
        tson::Tileset* tileset = tileObject.getTile()->getTileset();
        bool hasAnimation = tileObject.getTile()->getAnimation().any();
        tson::Rect drawingRect;

        if (!hasAnimation)
            drawingRect = tileObject.getDrawingRect();
        else
        {
            //tileObject.getTile()->getAnimation().update(m_timeDelta.asMilliseconds());
            uint32_t ownerId = tileObject.getTile()->getId();
            if (m_animationUpdateQueue.count(ownerId) == 0) //This is only built once to track all tile IDs with animations
                m_animationUpdateQueue[ownerId] = &tileObject.getTile()->getAnimation();

            uint32_t tileId = tileObject.getTile()->getAnimation().getCurrentTileId();
            tson::Tile* animatedTile = tileset->getTile(tileId);
            drawingRect = animatedTile->getDrawingRect();
        }

        const sf::Texture* texture = nullptr;
        if (tileset->getType() == tson::TilesetType::ImageCollectionTileset)
        {
            tson::Tile* tile = tileObject.getTile();
            if (tile->getFlipFlags() != tson::TileFlipFlags::None)
            {
                // <add comment>
                tile = tileset->getTile(tile->getId());
            }
            fs::path tilesetPath = getImagePath(*tile);
            texture = getTexture(tilesetPath.generic_string());
        }
        else if (tileset->getType() == tson::TilesetType::ImageTileset)
        {
            fs::path tilesetPath = getImagePath(*tileset);
            texture = getTexture(tilesetPath.generic_string());
        }
        
        if (texture != nullptr)
        {
            sf::Vector2f position{ tileObject.getPosition().x, tileObject.getPosition().y };
            sf::Vector2f positionOffset { static_cast<float>(m_positionOffset.x), 
                                          static_cast<float>(m_positionOffset.y) };
            
            tson::Tile* tile = tileObject.getTile();
            drawTexture(*texture, 
                        { drawingRect.x, drawingRect.y, drawingRect.width, drawingRect.height },
                        position + positionOffset, 
                        tileHeight, 
                        tile->hasFlipFlags(tson::TileFlipFlags::Horizontally),
                        tile->hasFlipFlags(tson::TileFlipFlags::Vertically),
                        tile->hasFlipFlags(tson::TileFlipFlags::Diagonally));
        }
    }
}

void SfmlDemoManager::drawImageLayer(tson::Layer &layer)
{
    sf::Sprite *sprite = storeAndLoadImage(layer.getImage(), {layer.getOffset().x, layer.getOffset().y});
    if(sprite != nullptr)
        m_window.draw(*sprite);
}

void SfmlDemoManager::drawObjectLayer(tson::Layer& layer)
{
    auto* map = layer.getMap();
    for (auto& obj : layer.getObjects())
    {
        switch (obj.getObjectType())
        {
        case tson::ObjectType::Object:
        {
            tson::Tileset* tileset = layer.getMap()->getTilesetByGid(obj.getGid());
            
            const sf::Texture* texture = nullptr;
            sf::IntRect textureRegion;

            if (tileset->getType() == tson::TilesetType::ImageCollectionTileset)
            {
                // Look up tile via object global id
                uint32_t id = obj.getGid() - tileset->getFirstgid() + 1;
                tson::Tile* tile = tileset->getTile(id);
                fs::path tilesetPath = getImagePath(*tile);
                texture = getTexture(tilesetPath.generic_string());

                textureRegion = { 0, 0, obj.getSize().x, obj.getSize().y };
            }
            else if (tileset->getType() == tson::TilesetType::ImageTileset)
            {
                fs::path tilesetPath = getImagePath(*tileset);
                texture = getTexture(tilesetPath.generic_string());

                sf::Vector2f offset = getTileOffset(obj.getGid(), map, tileset);
                textureRegion = { static_cast<int32_t>(offset.x),
                                  static_cast<int32_t>(offset.y),
                                  map->getTileSize().x,
                                  map->getTileSize().y };
            }
            
            std::string name = obj.getName();
            sf::Vector2f position = { (float)obj.getPosition().x + (float)m_positionOffset.x, (float)obj.getPosition().y + (float)m_positionOffset.y };
            if (texture != nullptr)
            {
                sf::Sprite sprite(*texture, textureRegion);

                sf::Vector2f scale = sprite.getScale();
                sf::Vector2f originalScale = scale;
                float rotation = sprite.getRotation();
                float originalRotation = rotation;
                sf::Vector2f origin{ ((float)m_map->getTileSize().x) / 2, ((float)map->getTileSize().y) / 2 };

                if(obj.hasFlipFlags(tson::TileFlipFlags::Horizontally))
                    scale.x = -scale.x;
                if(obj.hasFlipFlags(tson::TileFlipFlags::Vertically))
                    scale.y = -scale.y;
                if(obj.hasFlipFlags(tson::TileFlipFlags::Diagonally))
                    rotation += 90.f;

                position = {position.x + origin.x, position.y + origin.y };
                sprite.setOrigin(origin);              
                sprite.setPosition({ position.x, position.y - map->getTileSize().y });
                sprite.setScale(scale);
                sprite.setRotation(rotation);

                m_window.draw(sprite);

                sprite.setScale(originalScale);       //Since we used a shared sprite for this example, we must reset the scale.
                sprite.setRotation(originalRotation); //Since we used a shared sprite for this example, we must reset the rotation.
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
            m_demoText.setPosition({ (float)obj.getPosition().x, (float)obj.getPosition().y });
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
 * Getter to return a cached texture keyed of the image filepath.
 * @param image
 * @return sf::Texture
 */
const sf::Texture* SfmlDemoManager::getTexture(const std::string& image)
{
    // Workaround: Using only texture data to avoid resetting sprite transform after each render.
    // Note: sf::Sprite is lightweight and inexpensive to create.
    sf::Sprite* sprite = storeAndLoadImage(image, { 0, 0 });
    if (sprite)
    {
        return sprite->getTexture();
    }
    return nullptr;
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
        if(fs::exists(path) && fs::is_regular_file(path))
        {
            std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
            bool imageFound = tex->loadFromFile(path.generic_string());
            if(imageFound)
            {
                std::unique_ptr<sf::Sprite> spr = std::make_unique<sf::Sprite>();
                spr->setTexture(*tex);
                spr->setPosition(position);
                m_textures[image] = std::move(tex);
                m_sprites[image] = std::move(spr);
            }
        }
        else
            std::cout << "Could not find: " << path.generic_string() << std::endl;
    }

    if(m_sprites.count(image) > 0)
        return m_sprites[image].get();

    return nullptr;
}

sf::Vector2f SfmlDemoManager::getTileOffset(int tileId, tson::Map *map, tson::Tileset* tileset)
{

    //tson::Tileset* tileset = map->getTileset("demo-tileset");

    uint32_t firstId = tileset->getFirstgid(); //First tile id of the tileset
    int columns = tileset->getColumns(); //For the demo map it is 8.
    int rows = tileset->getTileCount() / columns;
    uint32_t lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

    //With this, I know that it's related to the tileset above (though I only have one tileset)
    if (tileId >= firstId && tileId <= lastId)
    {
        uint32_t baseTilePosition = (tileId - firstId);

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
template <typename T>
fs::path SfmlDemoManager::getImagePath(const T &tileset)
{
    fs::path path = fs::path(fs::path("../") / tileset.getImage().filename());
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
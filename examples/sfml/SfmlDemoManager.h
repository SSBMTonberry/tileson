//
// Created by robin on 22.09.2019.
//

#ifndef TILESON_SFMLDEMOMANAGER_H
#define TILESON_SFMLDEMOMANAGER_H


#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

#include <map>
#include "vera_font.h"

#include "../../include/external/nlohmann.hpp"
//#include "../../include/tileson.h"
#include "../../extras/pocketlzma.hpp"
#include "../../tileson.hpp"
//#include "../../include/external/json.hpp"
//#include "../../single_include/tileson_min.hpp"

#if __clang__
#include <mach-o/dyld.h>
#endif

class SfmlDemoManager
{
    public:
        SfmlDemoManager() = default;

        bool initialize( const sf::Vector2i &windowSize, const sf::Vector2i &resolution = {400, 240},
                         const std::string &title = "SFML Tileson demo",
                         const fs::path &basePath = fs::path("../../../content/test-maps/"));
        std::unique_ptr<tson::Map> parseMap(const std::string &filename = "ultimate_test.json", std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor = nullptr);
        bool parseProject(const std::string &filename = "test.tiled-project");
        void run();

    private:
        #if __clang__
        fs::path getMacApplicationFolder(bool isAppPath);
        #endif
        
        void drawMap();
        void drawImgui();

        static constexpr int CURRENT_VERSION = 120; //v1.2.0

        void drawLayer(tson::Layer& layer);
        void drawTileLayer(tson::Layer& layer);//, tson::Tileset* tileset);
        void drawImageLayer(tson::Layer& layer);
        void drawObjectLayer(tson::Layer& layer);
        void drawTexture(const sf::Texture& texture, const sf::IntRect& textureRegion, const sf::Vector2f& position, 
                         int32_t tileHeight, bool hortFlip, bool vertFlip, bool diagonalFlip);

        sf::Vector2f getTileOffset(int tileId, tson::Map *map, tson::Tileset* tileset);


        const sf::Texture* getTexture(const std::string& image);
        sf::Sprite * storeAndLoadImage(const std::string &image, const sf::Vector2f &position);
        template <typename T>
        fs::path getImagePath(const T &tileset);
        void updateAnimations();

        fs::path m_basePath {};
        sf::RenderWindow m_window;
        std::unique_ptr<tson::Map> m_map;
        std::unique_ptr<tson::Map> m_imageCollectionTilesetMap;
        std::unique_ptr<tson::Map> m_marginSpaceMap;

        std::map<std::string, std::unique_ptr<tson::Map>> m_projectMaps; //Non-world maps in project

        std::vector<std::unique_ptr<tson::Map>> m_worldMaps; //World-related maps in project
        std::vector<tson::WorldMapData> m_worldData; //World-related data in project
        std::vector<bool> m_worldVisibilityFlags; //World-related data in project
        std::vector<std::string> m_worldMapInfo;
        std::vector<std::string> m_projectMapInfo;

        tson::Map *m_currentMap;
        std::string m_currentInfo;
        bool m_isImguiSizeSet = false;

        tson::Project m_project; //Can also use Nlohmann like this: tson::Project m_project {std::make_unique<tson::NlohmannJson>()};

        sf::Font m_font;
        sf::Text m_demoText;
        sf::Time m_timeDelta;

        int m_mapIndex {0};
        const int m_maxMapIndex {5};
        
        tson::Vector2i m_positionOffset {0,0}; //Used for world related stuff

        std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
        std::map<std::string, std::unique_ptr<sf::Sprite>> m_sprites;
        std::map<uint32_t, tson::Animation*> m_animationUpdateQueue;
};


#endif //TILESON_SFMLDEMOMANAGER_H

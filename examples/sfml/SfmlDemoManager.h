//
// Created by robin on 22.09.2019.
//

#ifndef TILESON_SFMLDEMOMANAGER_H
#define TILESON_SFMLDEMOMANAGER_H


#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "Tileson.h"

class SfmlDemoManager
{
    public:
        SfmlDemoManager() = default;

        void initialize( const sf::Vector2i &windowSize, const sf::Vector2i &resolution = {400, 240}, const std::string &title = "SFML Tileson demo", const fs::path &basePath = fs::path("../../../content/test-maps/"));
        bool parseMap(const std::string &filename = "ultimate_test.json");
        void run();

    private:
        void drawMap();

        void drawTileLayer(tson::Layer& layer, tson::Tileset* tileset);

        fs::path m_basePath {};
        sf::RenderWindow m_window;
        tson::Map m_map;

        sf::Texture m_demoTilesetTexture;
        sf::Sprite m_demoTilesetSprite;
};


#endif //TILESON_SFMLDEMOMANAGER_H

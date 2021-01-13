//
// Created by robin on 21.09.2019.
//

#include "../external_libs/catch.hpp"
//#include "../src/Tileson.h"
//#include "../include/tileson.hpp"
#include "../TilesonConfig.h"
#include "../single_include/tileson.hpp"
//#include "../include/tileson.h"

#include "tson_files_mapper.h"
#include "../TilesonConfig.h"

TEST_CASE( "Nullptr error on getposition when parsing json (Issue #17)", "[help][issue]")
{
    tson::Tileson t;

    fs::path path {"../../content/test-maps/issues/issue_17.json"};

    std::unique_ptr<tson::Map> map = t.parse(path); // <== this is where I get the nullptr error

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& layer : map->getLayers())
        {
            if(layer.getType() == tson::LayerType::TileLayer)
            {
                for (const auto &[id, obj] : layer.getTileObjects())
                {
                    tson::Vector2f groundPos = tson::Vector2f(obj.getPosition().x, obj.getPosition().y);
                    tson::Vector2f groundSize = tson::Vector2f(obj.getTile()->getTileSize().x, obj.getTile()->getTileSize().y);

                    //platforms.push_back(Platform(&groundtexture, groundSize, groundPos));
                }
            }
        }
    }
    else
    {
        std::cout << map->getStatusMessage() << std::endl;
    }
}

TEST_CASE( "Help a fellow programmer in need - expect solution (Issue #4)", "[help][issue]")
{
    std::unique_ptr<tson::Map> jsCarte;
    //std::string pathCarte;
    tson::Tileson jsTileson;
    tson::Layer* tileLayer;
    tson::Tileset* tileset;

    fs::path pathCarte {"../../content/test-maps/issues/Preluda3.json"};
    jsCarte = jsTileson.parse(pathCarte);

    if (jsCarte->getStatus() == tson::ParseStatus::OK)
    {
        //std::cout << "C'est bon, j'ai chargé la carte : " << pathCarte << std::endl;
    }
    else
    {
        REQUIRE( true ); //Skip this if map is not found
        return;
    }

    //On met en variable la layer et le tileset correspondant
    tileLayer = jsCarte->getLayer("WL");
    ////On met en mémoire le tileset
    tileset = jsCarte->getTileset("ground_tiles");

    //sfText.loadFromFile("C:/Users/remy/source/repos/PROJET JEU/PRO2/Tiles/TS/ground_tiles.png");


    //On récupère le premier ID de la tile du Tileset
    int firstId = tileset->getFirstgid(); //First tile id of the tileset

    //On récupère le nombre de colonne
    int columns = tileset->getColumns(); //For the demo map it is 8.
    //On récupère le dernier ID (=premier ID + nombre de tile -1)
    int lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;

    //On vérifie que le layer est bien un tileLayer
    if (tileLayer->getType() == tson::LayerType::TileLayer)
    {
        //pos = position in tile units
        for (auto& [pos, tile] : tileLayer->getTileData()) //Loops through absolutely all existing tiles
        {

            fs::path imagePath;

            std::string pathStr;
            //With this, I know that it's related to the tileset above (though I only have one tileset)
            //Si l'ID de la tile est compris entre le premier et le dernier ID inclus
            if (tile->getId() >= firstId && tile->getId() <= lastId)
            {
                imagePath = tileset->getImagePath();
                pathStr = imagePath.u8string();
            }

            //Get position in pixel units --> Position de la Tile
            tson::Vector2i position = { std::get<0>(pos) * jsCarte->getTileSize().x,std::get<1>(pos) * jsCarte->getTileSize().y };
            int tileId = tile->getId();
            //The ID can be used to calculate offset on its related tileset image.
            int offsetX = (tileId % columns) * jsCarte->getTileSize().x;
            int offsetY = (tileId / columns) * jsCarte->getTileSize().y;

            REQUIRE(tile != nullptr);
            //Now you can use your library of choice to load the image (like SFML), then set the offset
            //to get the right image representation of the tile.

            //Removed due to be unrelated to the parsing itself
            //sfSprite.setTexture(sfText);
            //sfSprite.setTextureRect(sf::IntRect(offsetX - 16, offsetY, 16, 16));
            //sfSprite.setPosition(position.x, position.y);
            //ns_Fenst::fPrinc.draw(sfSprite);
        }
        //ns_Fenst::fPrinc.display();
    }
}
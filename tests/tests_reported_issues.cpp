//
// Created by robin on 21.09.2019.
//

#include "../external_libs/catch.hpp"
#include "../TilesonConfig.h"

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../tileson.hpp"
#else
    #include "../include/tileson.h"
#endif

#include "tson_files_mapper.h"
#include "../TilesonConfig.h"
#include "TestTools.hpp"

TEST_CASE( "Nullptr error on getposition when parsing json (Issue #17)", "[help][issue]")
{
    /* See comment in issue 17 */

#if 0
    tson::Tileson t;

    fs::path path = GetPathWithBase(fs::path("test-maps/issues/issue_17.json"));

    std::unique_ptr<tson::Map> map = t.parse(path); // <== this is where I get the nullptr error

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        for (auto& layer : map->getLayers())
        {
            if(layer.getType() == tson::LayerType::TileLayer)
            {
                for ([[maybe_unused]] auto &[id, obj] : layer.getTileObjects())
                {
                    #if __GNUC__ == 7 //[[maybe_unused]] is not implemented for structured bindings for GCC7, so do this hack to make GCC7 happy
                    (void) id;
                    #endif
                    tson::Vector2f(obj.getPosition().x, obj.getPosition().y);
                    tson::Vector2f(static_cast<float>(obj.getTile()->getTileSize().x), static_cast<float>(obj.getTile()->getTileSize().y));

                    //platforms.push_back(Platform(&groundtexture, groundSize, groundPos));
                }
            }
        }
    }
    else
    {
        std::cout << map->getStatusMessage() << std::endl;
    }
#endif
}

TEST_CASE( "Tile ObjectGroup's not set properly if one or more tiles have no properties or objects (Issue #46)", "[help][issue]")
{
    tson::Tileson t;

    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/issues/issue_46_map.json"));

    std::unique_ptr<tson::Map> map = t.parse(pathToUse); // <== this is where I get the nullptr error
    if (map->getStatus() == tson::ParseStatus::OK)
    {
        auto &tileset = map->getTilesets()[0];

        REQUIRE(tileset.getTile(1)->getObjectgroup().getObjects().size() == 1);
        REQUIRE(tileset.getTile(2)->getObjectgroup().getObjects().size() == 1);
        REQUIRE(tileset.getTile(3)->getObjectgroup().getObjects().size() == 3);
        REQUIRE(tileset.getTile(4)->getObjectgroup().getObjects().size() == 3);
        REQUIRE(tileset.getTile(5)->getObjectgroup().getObjects().size() == 1);
        REQUIRE(tileset.getTile(6)->getObjectgroup().getObjects().size() == 3);

        REQUIRE(tileset.getTile(7)->getObjectgroup().getObjects().empty());
        REQUIRE(tileset.getTile(8)->getObjectgroup().getObjects().empty());

        REQUIRE(tileset.getTile(9)->getObjectgroup().getObjects().size() == 1);

        REQUIRE(tileset.getTile(10)->getObjectgroup().getObjects().empty());
        REQUIRE(tileset.getTile(11)->getObjectgroup().getObjects().empty());
        REQUIRE(tileset.getTile(12)->getObjectgroup().getObjects().empty());

        REQUIRE(tileset.getTile(13)->getObjectgroup().getObjects().size() == 1);

        REQUIRE(tileset.getTile(14)->getObjectgroup().getObjects().empty());
        REQUIRE(tileset.getTile(15)->getObjectgroup().getObjects().empty());

        REQUIRE(tileset.getTile(16)->getObjectgroup().getObjects().size() == 1);

        REQUIRE(tileset.getTile(17)->getObjectgroup().getObjects().empty());
        REQUIRE(tileset.getTile(18)->getObjectgroup().getObjects().empty());

        REQUIRE(tileset.getTile(19)->getObjectgroup().getObjects().size() == 1);
        REQUIRE(tileset.getTile(20)->getObjectgroup().getObjects().size() == 1);
        REQUIRE(tileset.getTile(21)->getObjectgroup().getObjects().size() == 1);
    }
    else
    {
        std::cout << map->getStatusMessage() << std::endl;
        REQUIRE(false);
    }
}

TEST_CASE( "Help a fellow programmer in need - expect solution (Issue #4)", "[help][issue]")
{
    std::unique_ptr<tson::Map> jsCarte;
    //std::string pathCarte;
    tson::Tileson jsTileson;
    tson::Layer* tileLayer;

    fs::path pathCarte = GetPathWithBase(fs::path("test-maps/issues/Preluda3.json"));

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
    jsCarte->getTileset("ground_tiles");

    //On vérifie que le layer est bien un tileLayer
    if (tileLayer->getType() == tson::LayerType::TileLayer)
    {
        //pos = position in tile units
        for ([[maybe_unused]] auto& [pos, tile] : tileLayer->getTileData()) //Loops through absolutely all existing tiles
        {
            #if __GNUC__ == 7 //[[maybe_unused]] is not implemented for structured bindings for GCC7, so do this hack to make GCC7 happy
            (void) pos;
            #endif
            REQUIRE(tile != nullptr);
        }
    }
}

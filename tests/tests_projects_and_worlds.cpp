//
// Created by robin on 03.08.2020.
//


#include "../external_libs/catch.hpp"
#include "tileson.h"

TEST_CASE( "Parse world - Expect 4 maps and parsed data", "[project][world]" )
{
    fs::path pathLocal {"../../content/test-maps/project/world/test.world"};
    fs::path pathTravis {"../content/test-maps/project/world/test.world"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::World world {pathToUse};
    REQUIRE(world.getMapData().size() == 4);
    REQUIRE(world.get("not_exists") == nullptr);
    REQUIRE(world.get("w1.json") != nullptr);
    REQUIRE(world.get("w1.json")->size == tson::Vector2i (256, 128));
    REQUIRE(world.get("w1.json")->position == tson::Vector2i (-256, -128));
    REQUIRE(!world.onlyShowAdjacentMaps());
    REQUIRE(world.getType() == "world");
}

TEST_CASE( "Parse project - expect right number of files and folders", "[project][world]" )
{
    fs::path pathLocal {"../../content/test-maps/project/test.tiled-project"};
    fs::path pathTravis {"../content/test-maps/project/test.tiled-project"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::Project project {pathToUse};
    auto files = project.getFolders().at(0).getSubFolders().at(0).getFiles();
    std::string str;
    std::for_each(files.begin(), files.end(), [&](const auto &item) { str.append(item.generic_string()); });

    REQUIRE(project.getData().folders.size() == 2);
    REQUIRE(project.getFolders().size() == 2);
    REQUIRE(project.getFolders().at(0).getPath().filename() == "maps");
    REQUIRE(!project.getFolders().at(0).hasWorldFile());
    REQUIRE(project.getFolders().at(0).getFiles().size() == 3);
    REQUIRE(project.getFolders().at(0).getSubFolders().size() == 1);
    REQUIRE(project.getFolders().at(0).getSubFolders().at(0).getFiles().size() == 1);
    REQUIRE(project.getFolders().at(1).getPath().filename() == "world");
    REQUIRE(project.getFolders().at(1).hasWorldFile());
    REQUIRE(project.getFolders().at(1).getFiles().size() == 4);
    REQUIRE(project.getFolders().at(1).getSubFolders().empty());
}

TEST_CASE( "Parse project with class info in maps - expect right values", "[project][map]" )
{
    fs::path pathLocal {"../../content/test-maps/project/test.tiled-project"};
    fs::path pathTravis {"../content/test-maps/project/test.tiled-project"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::Project project {pathToUse};
    auto files = project.getFolders().at(0).getSubFolders().at(0).getFiles();
    std::string str;
    std::for_each(files.begin(), files.end(), [&](const auto &item) { str.append(item.generic_string()); });

    REQUIRE(project.getData().folders.size() == 2);
    REQUIRE(project.getFolders().size() == 2);
    REQUIRE(project.getFolders().at(0).getPath().filename() == "maps");
    REQUIRE(!project.getFolders().at(0).hasWorldFile());
    REQUIRE(project.getFolders().at(0).getFiles().size() == 3);
    REQUIRE(project.getFolders().at(0).getSubFolders().size() == 1);
    REQUIRE(project.getFolders().at(0).getSubFolders().at(0).getFiles().size() == 1);
    REQUIRE(project.getFolders().at(1).getPath().filename() == "world");
    REQUIRE(project.getFolders().at(1).hasWorldFile());
    REQUIRE(project.getFolders().at(1).getFiles().size() == 4);
    REQUIRE(project.getFolders().at(1).getSubFolders().empty());

    auto folderFiles = project.getFolders().at(0).getFiles();
    for(fs::path &f : folderFiles)
    {
        fs::path path = project.getFolders().at(0).getPath() / f.filename();
        std::string filename = f.filename().generic_string();
        if(filename == "map1.json")
        {
            tson::Tileson t {&project};
            REQUIRE(fs::exists(path));
            std::unique_ptr<tson::Map> m = t.parse(path);
            REQUIRE(m->getStatus() == tson::ParseStatus::OK);
            auto c = m->get<tson::TiledClass>("classdata");

            //Asserts
            int age = c.get<int>("Age");
            fs::path extraFile = c.get<fs::path>("ExtraFile");
            uint32_t myObject = c.get<uint32_t>("MyObject");
            std::string name = c.get<std::string>("Name");
            bool canDestroy = c.get<bool>("CanDestroy");
            float money = c.get<float>("MoneyInBag");
            tson::Colori color = c.get<tson::Colori>("ShoeColor");
            tson::EnumValue enumNumber = c.get<tson::EnumValue>("NumFlag");
            tson::EnumValue enumString = c.get<tson::EnumValue>("StrFlag");

            REQUIRE(age == 51);
            REQUIRE(extraFile.filename() == "ultimate_test_v1.5.json");
            REQUIRE(myObject == 1);
            REQUIRE(name == "Karen Modifini");
            REQUIRE(canDestroy);
            REQUIRE(tson::Tools::Equal(money, 16.9344f));
            REQUIRE(color == "#ff069504");
            REQUIRE(enumNumber.getValue() == 10);
            REQUIRE(enumString.getValue() == 6);
        }
    }
}
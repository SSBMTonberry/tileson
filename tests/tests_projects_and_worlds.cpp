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
            int age = c.get<int>("Age");
            REQUIRE(age == 51);
        }
    }
}
//
// Created by robin on 06.01.2021.
//

#include "../external_libs/catch.hpp"
#include "../include/external/nlohmann.hpp"
#include "../include/external/picojson.hpp"
#include "../include/tileson.h"


//#include "IJsonFakes.hpp"

//#include "../include/json/NlohmannJson.hpp"



TEST_CASE( "Test a nlohmann implementation of IJson", "[json][interface]" )
{
    nlohmann::json jstr = "{\n"
                       "    \"cornercolors\":[],\n"
                       "    \"edgecolors\":[\n"
                       "        {\n"
                       "            \"color\":\"#ff0000\",\n"
                       "            \"name\":\"Red\",\n"
                       "            \"probability\":1,\n"
                       "            \"tile\":-1\n"
                       "        }, \n"
                       "        {\n"
                       "            \"color\":\"#00ff00\",\n"
                       "            \"name\":\"Green\",\n"
                       "            \"probability\":1,\n"
                       "            \"tile\":-1\n"
                       "        }, \n"
                       "        {\n"
                       "            \"color\":\"#0000ff\",\n"
                       "            \"name\":\"Blue\",\n"
                       "            \"probability\":1,\n"
                       "            \"tile\":-1\n"
                       "        }, \n"
                       "        {\n"
                       "            \"color\":\"#ff7700\",\n"
                       "            \"name\":\"Orange\",\n"
                       "            \"probability\":1,\n"
                       "            \"tile\":-1\n"
                       "        }],\n"
                       "    \"name\":\"FirstWang\",\n"
                       "    \"properties\":[\n"
                       "        {\n"
                       "            \"name\":\"floating_wang\",\n"
                       "            \"type\":\"float\",\n"
                       "            \"value\":14.6\n"
                       "        }, \n"
                       "        {\n"
                       "            \"name\":\"is_wang\",\n"
                       "            \"type\":\"bool\",\n"
                       "            \"value\":true\n"
                       "        }],\n"
                       "    \"tile\":-1,\n"
                       "    \"wangtiles\":[\n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":0,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 1, 0, 1, 0, 3, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":1,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[1, 0, 1, 0, 1, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":2,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 3, 0, 1, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":3,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 1, 0, 1, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":4,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[2, 0, 2, 0, 1, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":8,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[1, 0, 1, 0, 3, 0, 3, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":9,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[2, 0, 1, 0, 1, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":10,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[1, 0, 3, 0, 3, 0, 1, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":16,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 3, 0, 3, 0, 3, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":17,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 3, 0, 3, 0, 3, 0]\n"
                       "        }, \n"
                       "        {\n"
                       "            \"dflip\":false,\n"
                       "            \"hflip\":false,\n"
                       "            \"tileid\":18,\n"
                       "            \"vflip\":false,\n"
                       "            \"wangid\":[3, 0, 3, 0, 1, 0, 1, 0]\n"
                       "        }]\n"
                       "}"_json;
    nlohmann::json *jptr = &jstr;
    std::unique_ptr<tson::IJson> json = std::make_unique<tson::NlohmannJson>(jptr);
    tson::IJson &j = *json;
    //std::vector<std::unique_ptr<tson::IJson>> wang = j->array("wangtiles");
    //std::vector<std::unique_ptr<tson::IJson>> props = j->array("properties");
    std::vector<std::unique_ptr<tson::IJson>> &wang = j.array("wangtiles");
    std::vector<std::unique_ptr<tson::IJson>> &props = j.array("properties");
    tson::IJson &w1 = j["wangtiles"];
    tson::IJson &p1 = j["properties"];
    //size_t nsize = sizeof (nlohmann::json);
    //size_t isize = sizeof (uint32_t);
    REQUIRE( j.get<std::string>("name") == "FirstWang");
    REQUIRE( props.at(0)->get<std::string>("name") == "floating_wang");
    REQUIRE( props.at(1)->get<std::string>("name") == "is_wang"); //type
    REQUIRE( j.array("properties").size() == 2);
    REQUIRE( j.array("wangtiles").size() == 11);

    //Changed in Nlohmann Json 3.9.1
    REQUIRE( p1.size() == 2); //3 -> 2
    REQUIRE( w1.size() == 11); //5 -> 11
    REQUIRE( j["properties"].size() == 2); //3 -> 2
    REQUIRE( j["wangtiles"].size() == 11); //5 -> 11

    REQUIRE( props.at(0)->size() == 3);
    REQUIRE( wang.at(0)->size() == 5);
    REQUIRE( wang.at(2)->size() == 5);
    REQUIRE( wang.at(2)->array("wangid").at(2)->get<uint32_t>() == 3);
    REQUIRE( wang.at(3)->array("wangid").at(2)->get<int>() == 1);
    REQUIRE( props.size() == 2);
    REQUIRE( wang.size() == 11);
}

TEST_CASE( "Test a picojson implementation of IJson", "[json][interface]" )
{

    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    std::unique_ptr<tson::IJson> j = std::make_unique<tson::PicoJson>();
    tson::IJson &json = *j;
    bool loadedFine = json.parse(pathToUse);
    REQUIRE(loadedFine);

    size_t c = json.count("compressionlevel");
    REQUIRE(c > 0);

    int compressionLevel = json["compressionlevel"].get<int>();
    REQUIRE(compressionLevel == -1);
}

TEST_CASE( "Test a Json11 implementation of IJson", "[json][interface]" )
{

    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    std::unique_ptr<tson::IJson> j = std::make_unique<tson::Json11>();
    tson::IJson &json = *j;
    bool loadedFine = json.parse(pathToUse);
    REQUIRE(loadedFine);

    size_t c = json.count("compressionlevel");
    REQUIRE(c > 0);

    int compressionLevel = json["compressionlevel"].get<int>();
    REQUIRE(compressionLevel == -1);
}
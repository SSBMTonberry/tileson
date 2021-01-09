//
// Created by robin on 06.01.2021.
//

#include "../external_libs/catch.hpp"
#include "../include/tileson.h"
//#include "IJsonFakes.hpp"
#include "../include/common/NlohmannJson.hpp"



//TEST_CASE( "Test a simple FAKE-implementation of IJson - Expect OK.", "[json][interface]" ) {
//    std::unique_ptr<tson::IJson> j = std::make_unique<FakeSon>();
//    //tson::IJson &f = j->operator[]("lol");
//    //REQUIRE( f.count("test") == 1 );
//    REQUIRE( j->count("test") == 1 );
//    REQUIRE( j->count("best") == 0 );
//    REQUIRE( j->any("test"));
//    REQUIRE( !j->any("best"));
//    REQUIRE( j->isObject());
//
//    REQUIRE( j->get<int32_t>("test") == -32 );
//    REQUIRE( j->get<int32_t>("best") == -1 );
//    REQUIRE( j->get<uint32_t>("test") == 32 );
//    REQUIRE( j->get<uint32_t>("best") == 0 );
//    REQUIRE( j->get<int64_t>("test") == -64 );
//    REQUIRE( j->get<int64_t>("best") == -1 );
//    REQUIRE( j->get<uint64_t>("test") == 64 );
//    REQUIRE( j->get<uint64_t>("best") == 0 );
//    REQUIRE( j->get<double>("test") > 1337.0 );
//    REQUIRE( j->get<double>("best") < 2 );
//    REQUIRE( j->get<std::string>("test") == "Test" );
//    REQUIRE( j->get<std::string>("best") == "No value" );
//    REQUIRE( j->get<bool>("test"));
//    REQUIRE( !j->get<bool>("best"));
//    REQUIRE( j->get<nullptr_t>("whatever") == nullptr);
//}

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
    std::vector<std::unique_ptr<tson::IJson>> wang = j.array("wangtiles");
    std::vector<std::unique_ptr<tson::IJson>> props = j.array("properties");
    tson::IJson &w1 = j["wangtiles"];
    tson::IJson &p1 = j["properties"];
    //size_t nsize = sizeof (nlohmann::json);
    //size_t isize = sizeof (uint32_t);
    REQUIRE( j.get<std::string>("name") == "FirstWang");
    REQUIRE( props.at(0)->get<std::string>("name") == "floating_wang");
    REQUIRE( props.at(1)->get<std::string>("name") == "is_wang"); //type
    REQUIRE( j.array("properties").size() == 2);
    REQUIRE( j.array("wangtiles").size() == 11);
    REQUIRE( p1.size() == 3);
    REQUIRE( w1.size() == 5);
    REQUIRE( j["properties"].size() == 3);
    REQUIRE( j["wangtiles"].size() == 5);
    REQUIRE( props.at(0)->size() == 3);
    REQUIRE( wang.at(0)->size() == 5);
    REQUIRE( wang.at(2)->size() == 5);
    REQUIRE( wang.at(2)->array("wangid").at(2)->get<uint32_t>() == 3);
    REQUIRE( wang.at(3)->array("wangid").at(2)->get<int>() == 1);
    REQUIRE( props.size() == 2);
    REQUIRE( wang.size() == 11);
}
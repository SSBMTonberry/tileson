//
// Created by robin on 21.07.22.
//

#include "../external_libs/catch.hpp"

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../single_include/tileson.hpp"
#else
    #include "../include/tileson.h"
#endif

enum class TestEnumNumberFlags
{
    None = 0,
    HasCalculatorFlag = 1 << 0,
    HasBombFlag = 1 << 1,
    HasHumorFlag = 1 << 2,
    HasInvisibilityFlag = 1 << 3
};

TEST_CASE( "Parse an int enum definition without flags - expect correct enum value checks", "[enum][int]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 3,\n"
                       "    \"name\": \"TestEnumNumber\",\n"
                       "    \"storageType\": \"int\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
                       "        \"None\",\n"
                       "        \"CreateNumber\",\n"
                       "        \"DeleteNumber\",\n"
                       "        \"UpdateNumber\",\n"
                       "        \"GetNumber\"\n"
                       "    ],\n"
                       "    \"valuesAsFlags\": false\n"
                       "}";

    std::string error;
    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::EnumDefinition def {*js};

    REQUIRE(def.getId() == 3);
    REQUIRE(def.getName() == "TestEnumNumber");
    REQUIRE(def.getStorageType() == tson::EnumStorageType::Int);
    REQUIRE(!def.hasValuesAsFlags());

    REQUIRE(def.getValue(1) == "CreateNumber");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "DeleteNumber");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(3) == "UpdateNumber");
    REQUIRE(def.exists(3));
    REQUIRE(def.getValue(4) == "GetNumber");
    REQUIRE(def.exists(4));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("CreateNumber") == 1);
    REQUIRE(def.exists("CreateNumber"));
    REQUIRE(def.getValue("DeleteNumber") == 2);
    REQUIRE(def.exists("DeleteNumber"));
    REQUIRE(def.getValue("UpdateNumber") == 3);
    REQUIRE(def.exists("UpdateNumber"));
    REQUIRE(def.getValue("GetNumber") == 4);
    REQUIRE(def.exists("GetNumber"));
    REQUIRE(def.getValue("None") == 0);
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"CreateNumber", &def};
    tson::EnumValue strV2 {"DeleteNumber", &def};
    tson::EnumValue strV3 {"UpdateNumber", &def};
    tson::EnumValue strV4 {"GetNumber", &def};
    tson::EnumValue numV1 {1, &def};
    tson::EnumValue numV2 {2, &def};
    tson::EnumValue numV3 {3, &def};
    tson::EnumValue numV4 {4, &def};

    REQUIRE(strV1.getValue() == 1);
    REQUIRE(strV2.getValue() == 2);
    REQUIRE(strV3.getValue() == 3);
    REQUIRE(strV4.getValue() == 4);
    REQUIRE(strV1.hasFlag(1));
    REQUIRE(strV2.hasFlag(2));
    REQUIRE(strV3.hasFlag(3));
    REQUIRE(strV4.hasFlag(4));
    REQUIRE(!strV4.hasFlag(97));
    REQUIRE(numV1.getName() == "CreateNumber");
    REQUIRE(numV2.getName() == "DeleteNumber");
    REQUIRE(numV3.getName() == "UpdateNumber");
    REQUIRE(numV4.getName() == "GetNumber");
    REQUIRE(numV1.hasFlag(1));
    REQUIRE(numV2.hasFlag(2));
    REQUIRE(numV3.hasFlag(3));
    REQUIRE(numV4.hasFlag(4));
    REQUIRE(!numV4.hasFlag(97));
}

TEST_CASE( "Parse an int enum definition with flags - expect correct enum value checks", "[enum][int][flags]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 4,\n"
                       "    \"name\": \"TestEnumNumberFlags\",\n"
                       "    \"storageType\": \"int\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
                       "        \"None\",\n"
                       "        \"HasCalculatorFlag\",\n"
                       "        \"HasBombFlag\",\n"
                       "        \"HasHumorFlag\",\n"
                       "        \"HasInvisibilityFlag\"\n"
                       "    ],\n"
                       "    \"valuesAsFlags\": true\n"
                       "}";

    std::string error;
    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::EnumDefinition def {*js};

    REQUIRE(def.getId() == 4);
    REQUIRE(def.getName() == "TestEnumNumberFlags");
    REQUIRE(def.getStorageType() == tson::EnumStorageType::Int);
    REQUIRE(def.hasValuesAsFlags());

    REQUIRE(def.getValue(1) == "HasCalculatorFlag");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "HasBombFlag");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(4) == "HasHumorFlag");
    REQUIRE(def.exists(4));
    REQUIRE(def.getValue(8) == "HasInvisibilityFlag");
    REQUIRE(def.exists(8));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("HasCalculatorFlag") == 1);
    REQUIRE(def.exists("HasCalculatorFlag"));
    REQUIRE(def.getValue("HasBombFlag") == 2);
    REQUIRE(def.exists("HasBombFlag"));
    REQUIRE(def.getValue("HasHumorFlag") == 4);
    REQUIRE(def.exists("HasHumorFlag"));
    REQUIRE(def.getValue("HasInvisibilityFlag") == 8);
    REQUIRE(def.exists("HasInvisibilityFlag"));
    REQUIRE(def.getValue("None") == 0);
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"HasCalculatorFlag", &def};
    tson::EnumValue strV2 {"HasBombFlag", &def};
    tson::EnumValue strV3 {"HasHumorFlag", &def};
    tson::EnumValue strV4 {"HasInvisibilityFlag", &def};
    tson::EnumValue numV1 {1, &def};
    tson::EnumValue numV2 {2, &def};
    tson::EnumValue numV3 {4, &def};
    tson::EnumValue numV4 {8, &def};

    REQUIRE(strV1.getValue() == 1);
    REQUIRE(strV2.getValue() == 2);
    REQUIRE(strV3.getValue() == 4);
    REQUIRE(strV4.getValue() == 8);
    REQUIRE(numV1.getName() == "HasCalculatorFlag");
    REQUIRE(numV2.getName() == "HasBombFlag");
    REQUIRE(numV3.getName() == "HasHumorFlag");
    REQUIRE(numV4.getName() == "HasInvisibilityFlag");

    tson::EnumValue strFlagV1 {"HasCalculatorFlag,HasHumorFlag,HasInvisibilityFlag", &def};
    tson::EnumValue strFlagV2 {"HasBombFlag,HasHumorFlag", &def};
    tson::EnumValue numFlagV1 {15, &def};
    tson::EnumValue numFlagV2 {6, &def};

    #error FINISH THIS^
}
//
// Created by robin on 21.07.22.
//

#include "../external_libs/catch.hpp"

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../tileson.hpp"
#else
    #include "../include/tileson.h"
#endif

#include "TestEnums.h"

TEST_CASE( "Parse an int enum definition without flags - expect correct enum value checks", "[enum][int]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 3,\n"
                       "    \"name\": \"TestEnumNumber\",\n"
                       "    \"storageType\": \"int\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
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

    REQUIRE(def.getValue(0) == "CreateNumber");
    REQUIRE(def.exists(0));
    REQUIRE(def.getValue(1) == "DeleteNumber");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "UpdateNumber");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(3) == "GetNumber");
    REQUIRE(def.exists(3));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("CreateNumber") == 0);
    REQUIRE(def.exists("CreateNumber"));
    REQUIRE(def.getValue("DeleteNumber") == 1);
    REQUIRE(def.exists("DeleteNumber"));
    REQUIRE(def.getValue("UpdateNumber") == 2);
    REQUIRE(def.exists("UpdateNumber"));
    REQUIRE(def.getValue("GetNumber") == 3);
    REQUIRE(def.exists("GetNumber"));
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"CreateNumber", &def};
    tson::EnumValue strV2 {"DeleteNumber", &def};
    tson::EnumValue strV3 {"UpdateNumber", &def};
    tson::EnumValue strV4 {"GetNumber", &def};
    tson::EnumValue numV1 {0, &def};
    tson::EnumValue numV2 {1, &def};
    tson::EnumValue numV3 {2, &def};
    tson::EnumValue numV4 {3, &def};

    REQUIRE(strV1.getValue() == 0);
    REQUIRE(strV2.getValue() == 1);
    REQUIRE(strV3.getValue() == 2);
    REQUIRE(strV4.getValue() == 3);
    REQUIRE(strV1.hasFlagValue(0));
    REQUIRE(strV2.hasFlagValue(1));
    REQUIRE(strV3.hasFlagValue(2));
    REQUIRE(strV4.hasFlagValue(3));
    REQUIRE(!strV4.hasFlagValue(97));
    REQUIRE(numV1.getValueName() == "CreateNumber");
    REQUIRE(numV2.getValueName() == "DeleteNumber");
    REQUIRE(numV3.getValueName() == "UpdateNumber");
    REQUIRE(numV4.getValueName() == "GetNumber");
    REQUIRE(numV1.hasFlagValue(0));
    REQUIRE(numV2.hasFlagValue(1));
    REQUIRE(numV3.hasFlagValue(2));
    REQUIRE(numV4.hasFlagValue(3));
    REQUIRE(!numV4.hasFlagValue(97));
}

TEST_CASE( "Parse an int enum definition with flags - expect correct enum value checks", "[enum][int][flags]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 4,\n"
                       "    \"name\": \"TestEnumNumberFlags\",\n"
                       "    \"storageType\": \"int\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
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
    REQUIRE(numV1.getValueName() == "HasCalculatorFlag");
    REQUIRE(numV2.getValueName() == "HasBombFlag");
    REQUIRE(numV3.getValueName() == "HasHumorFlag");
    REQUIRE(numV4.getValueName() == "HasInvisibilityFlag");

    tson::EnumValue strFlagV1 {"HasCalculatorFlag,HasHumorFlag,HasInvisibilityFlag", &def};
    tson::EnumValue strFlagV2 {"HasBombFlag,HasHumorFlag", &def};
    tson::EnumValue numFlagV1 {15, &def}; //All of them
    tson::EnumValue numFlagV2 {(uint32_t)(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasHumorFlag), &def};

    REQUIRE(strFlagV1.hasFlag(tson::TestEnumNumberFlags::HasCalculatorFlag | tson::TestEnumNumberFlags::HasHumorFlag |
                                               tson::TestEnumNumberFlags::HasInvisibilityFlag));
    REQUIRE(strFlagV1.hasFlag(tson::TestEnumNumberFlags::HasCalculatorFlag | tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(strFlagV1.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(strFlagV1.hasFlagValue(13));
    REQUIRE(strFlagV1.hasFlagValue(8));
    REQUIRE(!strFlagV1.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag | tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(!strFlagV1.hasFlagValue((uint32_t) (tson::TestEnumNumberFlags::HasBombFlag)));
    REQUIRE(strFlagV1.hasAnyFlag(tson::TestEnumNumberFlags::HasHumorFlag | tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(strFlagV1.hasAnyFlagValue(4));

    REQUIRE(strFlagV2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(!strFlagV2.hasFlag(tson::TestEnumNumberFlags::HasCalculatorFlag));

    REQUIRE(numFlagV1.hasFlag(tson::TestEnumNumberFlags::All));

    REQUIRE(numFlagV2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag));
    REQUIRE(!numFlagV2.hasFlag(tson::TestEnumNumberFlags::HasCalculatorFlag));
}

TEST_CASE( "Parse a string enum definition without flags - expect correct enum value checks", "[enum][string]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 2,\n"
                       "    \"name\": \"TestEnumString\",\n"
                       "    \"storageType\": \"string\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
                       "        \"CreatePlayer\",\n"
                       "        \"UpdatePlayer\",\n"
                       "        \"DeletePlayer\",\n"
                       "        \"GetPlayer\"\n"
                       "    ],\n"
                       "    \"valuesAsFlags\": false\n"
                       "}";

    std::string error;
    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::EnumDefinition def {*js};

    REQUIRE(def.getId() == 2);
    REQUIRE(def.getName() == "TestEnumString");
    REQUIRE(def.getStorageType() == tson::EnumStorageType::String);
    REQUIRE(!def.hasValuesAsFlags());

    REQUIRE(def.getValue(0) == "CreatePlayer");
    REQUIRE(def.exists(0));
    REQUIRE(def.getValue(1) == "UpdatePlayer");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "DeletePlayer");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(3) == "GetPlayer");
    REQUIRE(def.exists(3));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("CreatePlayer") == 0);
    REQUIRE(def.exists("CreatePlayer"));
    REQUIRE(def.getValue("UpdatePlayer") == 1);
    REQUIRE(def.exists("UpdatePlayer"));
    REQUIRE(def.getValue("DeletePlayer") == 2);
    REQUIRE(def.exists("DeletePlayer"));
    REQUIRE(def.getValue("GetPlayer") == 3);
    REQUIRE(def.exists("GetPlayer"));
    REQUIRE(def.getValue("None") == 0);
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"CreatePlayer", &def};
    tson::EnumValue strV2 {"DeletePlayer", &def};
    tson::EnumValue strV3 {"UpdatePlayer", &def};
    tson::EnumValue strV4 {"GetPlayer", &def};
    tson::EnumValue numV1 {0, &def};
    tson::EnumValue numV2 {1, &def};
    tson::EnumValue numV3 {2, &def};
    tson::EnumValue numV4 {3, &def};

    REQUIRE(strV1.getValue() == 0);
    REQUIRE(strV2.getValue() == 2);
    REQUIRE(strV3.getValue() == 1);
    REQUIRE(strV4.getValue() == 3);
    REQUIRE(strV1.hasFlagValue(0));
    REQUIRE(strV3.hasFlagValue(1));
    REQUIRE(strV2.hasFlagValue(2));
    REQUIRE(strV4.hasFlagValue(3));
    REQUIRE(!strV4.hasFlagValue(97));
    REQUIRE(numV1.getValueName() == "CreatePlayer");
    REQUIRE(numV2.getValueName() == "UpdatePlayer");
    REQUIRE(numV3.getValueName() == "DeletePlayer");
    REQUIRE(numV4.getValueName() == "GetPlayer");
    REQUIRE(numV1.hasFlagValue(0));
    REQUIRE(numV2.hasFlagValue(1));
    REQUIRE(numV3.hasFlagValue(2));
    REQUIRE(numV4.hasFlagValue(3));
    REQUIRE(!numV4.hasFlagValue(97));
}

TEST_CASE( "Parse a string enum definition with flags - expect correct enum value checks", "[enum][string][flags]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 5,\n"
                       "    \"name\": \"TestEnumStringFlags\",\n"
                       "    \"storageType\": \"string\",\n"
                       "    \"type\": \"enum\",\n"
                       "    \"values\": [\n"
                       "        \"HasCarFlag\",\n"
                       "        \"HasJobFlag\",\n"
                       "        \"HasHouseFlag\",\n"
                       "        \"HasMoneyFlag\"\n"
                       "    ],\n"
                       "    \"valuesAsFlags\": true\n"
                       "}";

    std::string error;
    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::EnumDefinition def {*js};

    REQUIRE(def.getId() == 5);
    REQUIRE(def.getName() == "TestEnumStringFlags");
    REQUIRE(def.getStorageType() == tson::EnumStorageType::String);
    REQUIRE(def.hasValuesAsFlags());

    REQUIRE(def.getValue(1) == "HasCarFlag");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "HasJobFlag");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(4) == "HasHouseFlag");
    REQUIRE(def.exists(4));
    REQUIRE(def.getValue(8) == "HasMoneyFlag");
    REQUIRE(def.exists(8));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("HasCarFlag") == 1);
    REQUIRE(def.exists("HasCarFlag"));
    REQUIRE(def.getValue("HasJobFlag") == 2);
    REQUIRE(def.exists("HasJobFlag"));
    REQUIRE(def.getValue("HasHouseFlag") == 4);
    REQUIRE(def.exists("HasHouseFlag"));
    REQUIRE(def.getValue("HasMoneyFlag") == 8);
    REQUIRE(def.exists("HasMoneyFlag"));
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"HasCarFlag", &def};
    tson::EnumValue strV2 {"HasJobFlag", &def};
    tson::EnumValue strV3 {"HasHouseFlag", &def};
    tson::EnumValue strV4 {"HasMoneyFlag", &def};
    tson::EnumValue numV1 {1, &def};
    tson::EnumValue numV2 {2, &def};
    tson::EnumValue numV3 {4, &def};
    tson::EnumValue numV4 {8, &def};

    REQUIRE(strV1.getValue() == 1);
    REQUIRE(strV2.getValue() == 2);
    REQUIRE(strV3.getValue() == 4);
    REQUIRE(strV4.getValue() == 8);
    REQUIRE(numV1.getValueName() == "HasCarFlag");
    REQUIRE(numV2.getValueName() == "HasJobFlag");
    REQUIRE(numV3.getValueName() == "HasHouseFlag");
    REQUIRE(numV4.getValueName() == "HasMoneyFlag");

    tson::EnumValue strFlagV1 {"HasCarFlag,HasHouseFlag,HasMoneyFlag", &def};
    tson::EnumValue strFlagV2 {"HasJobFlag,HasHouseFlag", &def};
    tson::EnumValue numFlagV1 {15, &def}; //All of them
    tson::EnumValue numFlagV2 {(uint32_t)(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag), &def};

    REQUIRE(strFlagV1.hasFlag(tson::TestEnumStringFlags::HasCarFlag | tson::TestEnumStringFlags::HasHouseFlag |
                              tson::TestEnumStringFlags::HasMoneyFlag));
    REQUIRE(strFlagV1.hasFlag(tson::TestEnumStringFlags::HasCarFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strFlagV1.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strFlagV1.hasFlagValue(13));
    REQUIRE(strFlagV1.hasFlagValue(8));
    REQUIRE(!strFlagV1.hasFlag(tson::TestEnumStringFlags::HasHouseFlag | tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(!strFlagV1.hasFlagValue((uint32_t) (tson::TestEnumStringFlags::HasJobFlag)));
    REQUIRE(strFlagV1.hasAnyFlag(tson::TestEnumStringFlags::HasHouseFlag | tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strFlagV1.hasAnyFlagValue(4));
    REQUIRE(strFlagV1.getValueNames().size() == 3);

    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!strFlagV2.hasFlag(tson::TestEnumStringFlags::HasCarFlag));
    REQUIRE(strFlagV2.getValueNames().size() == 2);

    std::vector<std::string> test = numFlagV1.getValueNames();

    REQUIRE(numFlagV1.hasFlag(tson::TestEnumStringFlags::All));
    REQUIRE(numFlagV1.getValueNames().size() == 4);

    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!numFlagV2.hasFlag(tson::TestEnumStringFlags::HasCarFlag));
    REQUIRE(numFlagV2.getValueNames().size() == 2);
}

TEST_CASE( "Parse a class definition with missing project file - expect all okay except enum values", "[class]" )
{
    std::string jstr = "{\n"
                       "    \"id\": 1,\n"
                       "    \"members\": [\n"
                       "        {\n"
                       "            \"name\": \"Age\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 49\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"CanDestroy\",\n"
                       "            \"type\": \"bool\",\n"
                       "            \"value\": true\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ExtraFile\",\n"
                       "            \"type\": \"file\",\n"
                       "            \"value\": \"../ultimate_test.json\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MoneyInBag\",\n"
                       "            \"type\": \"float\",\n"
                       "            \"value\": 16.9344\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MyObject\",\n"
                       "            \"type\": \"object\",\n"
                       "            \"value\": 39\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"Name\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"James Testolini\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"NumFlag\",\n"
                       "            \"propertyType\": \"TestEnumNumberFlags\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 10\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ShoeColor\",\n"
                       "            \"type\": \"color\",\n"
                       "            \"value\": \"#ff069504\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"StrFlag\",\n"
                       "            \"propertyType\": \"TestEnumStringFlags\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"HasJobFlag,HasHouseFlag\"\n"
                       "        }\n"
                       "    ],\n"
                       "    \"name\": \"TestClass\",\n"
                       "    \"type\": \"class\"\n"
                       "}";

    std::string error;
    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::TiledClass c {*js};

    REQUIRE(c.getId() == 1);
    REQUIRE(c.getName() == "TestClass");
    REQUIRE(c.getType() == "class");
    REQUIRE(c.getMembers().getSize() == 9);

    REQUIRE(c.getMember("Age")->getType() == tson::Type::Int);
    REQUIRE(c.getMember("Age")->getValue<int>() == 49);
    REQUIRE(c.get<int>("Age") == 49);
    REQUIRE(c.getMember("CanDestroy")->getType() == tson::Type::Boolean);
    REQUIRE(c.get<bool>("CanDestroy"));
    REQUIRE(c.getMember("ExtraFile")->getType() == tson::Type::File);
    REQUIRE(c.get<fs::path>("ExtraFile") == fs::path("../ultimate_test.json"));
    REQUIRE(c.getMember("MoneyInBag")->getType() == tson::Type::Float);
    REQUIRE(tson::Tools::Equal(c.get<float>("MoneyInBag"), 16.9344f));
    REQUIRE(c.getMember("MyObject")->getType() == tson::Type::Object);
    REQUIRE(c.get<uint32_t>("MyObject") == 39);
    REQUIRE(c.getMember("Name")->getType() == tson::Type::String);
    REQUIRE(c.get<std::string>("Name") == "James Testolini");
    REQUIRE(c.getMember("ShoeColor")->getType() == tson::Type::Color);
    tson::Colori color = c.get<tson::Colori>("ShoeColor");
    REQUIRE(color == "#ff069504");
    REQUIRE(color.a == 0xff);
    REQUIRE(color.r == 0x06);
    REQUIRE(color.g == 0x95);
    REQUIRE(color.b == 0x04);


    REQUIRE(c.getMember("StrFlag")->getType() == tson::Type::Enum);
    REQUIRE(c.getMember("NumFlag")->getType() == tson::Type::Enum);
}

TEST_CASE( "Parse a class definition with project file included - expect all okay", "[class]" )
{
    std::string error;

    std::string projectJson = "{\n"
                              "    \"automappingRulesFile\": \"\",\n"
                              "    \"commands\": [\n"
                              "    ],\n"
                              "    \"extensionsPath\": \"extensions\",\n"
                              "    \"folders\": [],\n"
                              "    \"objectTypesFile\": \"../objecttypes.json\",\n"
                              "    \"propertyTypes\": [\n"
                              "        {\n"
                              "            \"id\": 1,\n"
                              "            \"members\": [\n"
                              "                {\n"
                              "                    \"name\": \"Age\",\n"
                              "                    \"type\": \"int\",\n"
                              "                    \"value\": 49\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"CanDestroy\",\n"
                              "                    \"type\": \"bool\",\n"
                              "                    \"value\": true\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"ExtraFile\",\n"
                              "                    \"type\": \"file\",\n"
                              "                    \"value\": \"../ultimate_test.json\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"MoneyInBag\",\n"
                              "                    \"type\": \"float\",\n"
                              "                    \"value\": 16.9344\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"MyObject\",\n"
                              "                    \"type\": \"object\",\n"
                              "                    \"value\": 0\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"Name\",\n"
                              "                    \"type\": \"string\",\n"
                              "                    \"value\": \"James Testolini\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"NumFlag\",\n"
                              "                    \"propertyType\": \"TestEnumNumberFlags\",\n"
                              "                    \"type\": \"int\",\n"
                              "                    \"value\": 10\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"ShoeColor\",\n"
                              "                    \"type\": \"color\",\n"
                              "                    \"value\": \"#ff069504\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"StrFlag\",\n"
                              "                    \"propertyType\": \"TestEnumStringFlags\",\n"
                              "                    \"type\": \"string\",\n"
                              "                    \"value\": \"HasJobFlag,HasHouseFlag\"\n"
                              "                }\n"
                              "            ],\n"
                              "            \"name\": \"TestClass\",\n"
                              "            \"type\": \"class\"\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 3,\n"
                              "            \"name\": \"TestEnumNumber\",\n"
                              "            \"storageType\": \"int\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"CreateNumber\",\n"
                              "                \"DeleteNumber\",\n"
                              "                \"UpdateNumber\",\n"
                              "                \"GetNumber\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": false\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 4,\n"
                              "            \"name\": \"TestEnumNumberFlags\",\n"
                              "            \"storageType\": \"int\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"HasCalculatorFlag\",\n"
                              "                \"HasBombFlag\",\n"
                              "                \"HasHumorFlag\",\n"
                              "                \"HasInvisibilityFlag\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": true\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 2,\n"
                              "            \"name\": \"TestEnumString\",\n"
                              "            \"storageType\": \"string\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"CreatePlayer\",\n"
                              "                \"UpdatePlayer\",\n"
                              "                \"DeletePlayer\",\n"
                              "                \"GetPlayer\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": false\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 5,\n"
                              "            \"name\": \"TestEnumStringFlags\",\n"
                              "            \"storageType\": \"string\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"HasCarFlag\",\n"
                              "                \"HasJobFlag\",\n"
                              "                \"HasHouseFlag\",\n"
                              "                \"HasMoneyFlag\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": true\n"
                              "        }\n"
                              "    ]\n"
                              "}";

    json11::Json jproj = json11::Json::parse(projectJson, error);
    REQUIRE(error.empty());

    tson::Project p { std::make_unique<tson::Json11>(jproj) };
    p.parse();

    std::string jstr = "{\n"
                       "    \"id\": 1,\n"
                       "    \"members\": [\n"
                       "        {\n"
                       "            \"name\": \"Age\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 49\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"CanDestroy\",\n"
                       "            \"type\": \"bool\",\n"
                       "            \"value\": true\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ExtraFile\",\n"
                       "            \"type\": \"file\",\n"
                       "            \"value\": \"../ultimate_test.json\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MoneyInBag\",\n"
                       "            \"type\": \"float\",\n"
                       "            \"value\": 16.9344\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MyObject\",\n"
                       "            \"type\": \"object\",\n"
                       "            \"value\": 39\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"Name\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"James Testolini\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"NumFlag\",\n"
                       "            \"propertyType\": \"TestEnumNumberFlags\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 10\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ShoeColor\",\n"
                       "            \"type\": \"color\",\n"
                       "            \"value\": \"#ff069504\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"StrFlag\",\n"
                       "            \"propertyType\": \"TestEnumStringFlags\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"HasJobFlag,HasHouseFlag\"\n"
                       "        }\n"
                       "    ],\n"
                       "    \"name\": \"TestClass\",\n"
                       "    \"type\": \"class\"\n"
                       "}";

    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::TiledClass c {*js, &p};

    REQUIRE(c.getId() == 1);
    REQUIRE(c.getName() == "TestClass");
    REQUIRE(c.getType() == "class");
    REQUIRE(c.getMembers().getSize() == 9);

    REQUIRE(c.getMember("Age")->getType() == tson::Type::Int);
    REQUIRE(c.getMember("Age")->getValue<int>() == 49);
    REQUIRE(c.get<int>("Age") == 49);
    REQUIRE(c.getMember("CanDestroy")->getType() == tson::Type::Boolean);
    REQUIRE(c.get<bool>("CanDestroy"));
    REQUIRE(c.getMember("ExtraFile")->getType() == tson::Type::File);
    REQUIRE(c.get<fs::path>("ExtraFile") == fs::path("../ultimate_test.json"));
    REQUIRE(c.getMember("MoneyInBag")->getType() == tson::Type::Float);
    REQUIRE(tson::Tools::Equal(c.get<float>("MoneyInBag"), 16.9344f));
    REQUIRE(c.getMember("MyObject")->getType() == tson::Type::Object);
    REQUIRE(c.get<uint32_t>("MyObject") == 39);
    REQUIRE(c.getMember("Name")->getType() == tson::Type::String);
    REQUIRE(c.get<std::string>("Name") == "James Testolini");
    REQUIRE(c.getMember("ShoeColor")->getType() == tson::Type::Color);
    tson::Colori color = c.get<tson::Colori>("ShoeColor");
    REQUIRE(color == "#ff069504");
    REQUIRE(color.a == 0xff);
    REQUIRE(color.r == 0x06);
    REQUIRE(color.g == 0x95);
    REQUIRE(color.b == 0x04);

    tson::EnumValue numEnum = c.getMember("NumFlag")->getValue<tson::EnumValue>();
    tson::EnumValue strEnum = c.getMember("StrFlag")->getValue<tson::EnumValue>();

    REQUIRE(c.getMember("NumFlag")->getType() == tson::Type::Enum);
    REQUIRE(numEnum.getValue() == 10);
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasInvisibilityFlag));
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasInvisibilityFlag));
    REQUIRE(!numEnum.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag));

    REQUIRE(c.getMember("StrFlag")->getType() == tson::Type::Enum);
    REQUIRE(strEnum.getValue() == 6);
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!strEnum.hasFlag(tson::TestEnumStringFlags::HasMoneyFlag));
}

TEST_CASE( "Parse a class definition and copy it - expect same properties", "[class]" )
{
    std::string error;

    std::string projectJson = "{\n"
                              "    \"automappingRulesFile\": \"\",\n"
                              "    \"commands\": [\n"
                              "    ],\n"
                              "    \"extensionsPath\": \"extensions\",\n"
                              "    \"folders\": [],\n"
                              "    \"objectTypesFile\": \"../objecttypes.json\",\n"
                              "    \"propertyTypes\": [\n"
                              "        {\n"
                              "            \"id\": 1,\n"
                              "            \"members\": [\n"
                              "                {\n"
                              "                    \"name\": \"Age\",\n"
                              "                    \"type\": \"int\",\n"
                              "                    \"value\": 49\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"CanDestroy\",\n"
                              "                    \"type\": \"bool\",\n"
                              "                    \"value\": true\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"ExtraFile\",\n"
                              "                    \"type\": \"file\",\n"
                              "                    \"value\": \"../ultimate_test.json\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"MoneyInBag\",\n"
                              "                    \"type\": \"float\",\n"
                              "                    \"value\": 16.9344\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"MyObject\",\n"
                              "                    \"type\": \"object\",\n"
                              "                    \"value\": 0\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"Name\",\n"
                              "                    \"type\": \"string\",\n"
                              "                    \"value\": \"James Testolini\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"NumFlag\",\n"
                              "                    \"propertyType\": \"TestEnumNumberFlags\",\n"
                              "                    \"type\": \"int\",\n"
                              "                    \"value\": 10\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"ShoeColor\",\n"
                              "                    \"type\": \"color\",\n"
                              "                    \"value\": \"#ff069504\"\n"
                              "                },\n"
                              "                {\n"
                              "                    \"name\": \"StrFlag\",\n"
                              "                    \"propertyType\": \"TestEnumStringFlags\",\n"
                              "                    \"type\": \"string\",\n"
                              "                    \"value\": \"HasJobFlag,HasHouseFlag\"\n"
                              "                }\n"
                              "            ],\n"
                              "            \"name\": \"TestClass\",\n"
                              "            \"type\": \"class\"\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 3,\n"
                              "            \"name\": \"TestEnumNumber\",\n"
                              "            \"storageType\": \"int\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"CreateNumber\",\n"
                              "                \"DeleteNumber\",\n"
                              "                \"UpdateNumber\",\n"
                              "                \"GetNumber\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": false\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 4,\n"
                              "            \"name\": \"TestEnumNumberFlags\",\n"
                              "            \"storageType\": \"int\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"HasCalculatorFlag\",\n"
                              "                \"HasBombFlag\",\n"
                              "                \"HasHumorFlag\",\n"
                              "                \"HasInvisibilityFlag\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": true\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 2,\n"
                              "            \"name\": \"TestEnumString\",\n"
                              "            \"storageType\": \"string\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"CreatePlayer\",\n"
                              "                \"UpdatePlayer\",\n"
                              "                \"DeletePlayer\",\n"
                              "                \"GetPlayer\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": false\n"
                              "        },\n"
                              "        {\n"
                              "            \"id\": 5,\n"
                              "            \"name\": \"TestEnumStringFlags\",\n"
                              "            \"storageType\": \"string\",\n"
                              "            \"type\": \"enum\",\n"
                              "            \"values\": [\n"
                              "                \"HasCarFlag\",\n"
                              "                \"HasJobFlag\",\n"
                              "                \"HasHouseFlag\",\n"
                              "                \"HasMoneyFlag\"\n"
                              "            ],\n"
                              "            \"valuesAsFlags\": true\n"
                              "        }\n"
                              "    ]\n"
                              "}";

    json11::Json jproj = json11::Json::parse(projectJson, error);
    REQUIRE(error.empty());

    tson::Project p { std::make_unique<tson::Json11>(jproj) };
    p.parse();

    std::string jstr = "{\n"
                       "    \"id\": 1,\n"
                       "    \"members\": [\n"
                       "        {\n"
                       "            \"name\": \"Age\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 49\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"CanDestroy\",\n"
                       "            \"type\": \"bool\",\n"
                       "            \"value\": true\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ExtraFile\",\n"
                       "            \"type\": \"file\",\n"
                       "            \"value\": \"../ultimate_test.json\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MoneyInBag\",\n"
                       "            \"type\": \"float\",\n"
                       "            \"value\": 16.9344\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"MyObject\",\n"
                       "            \"type\": \"object\",\n"
                       "            \"value\": 39\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"Name\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"James Testolini\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"NumFlag\",\n"
                       "            \"propertyType\": \"TestEnumNumberFlags\",\n"
                       "            \"type\": \"int\",\n"
                       "            \"value\": 10\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"ShoeColor\",\n"
                       "            \"type\": \"color\",\n"
                       "            \"value\": \"#ff069504\"\n"
                       "        },\n"
                       "        {\n"
                       "            \"name\": \"StrFlag\",\n"
                       "            \"propertyType\": \"TestEnumStringFlags\",\n"
                       "            \"type\": \"string\",\n"
                       "            \"value\": \"HasJobFlag,HasHouseFlag\"\n"
                       "        }\n"
                       "    ],\n"
                       "    \"name\": \"TestClass\",\n"
                       "    \"type\": \"class\"\n"
                       "}";

    json11::Json j = json11::Json::parse(jstr, error);
    REQUIRE(error.empty());

    std::unique_ptr<tson::IJson> js = std::make_unique<tson::Json11>(j);
    tson::TiledClass c {*js, &p};
    tson::TiledClass c2 = c;

    REQUIRE(c.getId() == 1);
    REQUIRE(c.getName() == "TestClass");
    REQUIRE(c.getType() == "class");
    REQUIRE(c.getMembers().getSize() == 9);

    REQUIRE(c.getMember("Age")->getType() == tson::Type::Int);
    REQUIRE(c.getMember("Age")->getValue<int>() == 49);
    REQUIRE(c.get<int>("Age") == 49);
    REQUIRE(c.getMember("CanDestroy")->getType() == tson::Type::Boolean);
    REQUIRE(c.get<bool>("CanDestroy"));
    REQUIRE(c.getMember("ExtraFile")->getType() == tson::Type::File);
    REQUIRE(c.get<fs::path>("ExtraFile") == fs::path("../ultimate_test.json"));
    REQUIRE(c.getMember("MoneyInBag")->getType() == tson::Type::Float);
    REQUIRE(tson::Tools::Equal(c.get<float>("MoneyInBag"), 16.9344f));
    REQUIRE(c.getMember("MyObject")->getType() == tson::Type::Object);
    REQUIRE(c.get<uint32_t>("MyObject") == 39);
    REQUIRE(c.getMember("Name")->getType() == tson::Type::String);
    REQUIRE(c.get<std::string>("Name") == "James Testolini");
    REQUIRE(c.getMember("ShoeColor")->getType() == tson::Type::Color);
    tson::Colori color = c.get<tson::Colori>("ShoeColor");
    REQUIRE(color == "#ff069504");
    REQUIRE(color.a == 0xff);
    REQUIRE(color.r == 0x06);
    REQUIRE(color.g == 0x95);
    REQUIRE(color.b == 0x04);

    tson::EnumValue numEnum = c.getMember("NumFlag")->getValue<tson::EnumValue>();
    tson::EnumValue strEnum = c.getMember("StrFlag")->getValue<tson::EnumValue>();

    REQUIRE(c.getMember("NumFlag")->getType() == tson::Type::Enum);
    REQUIRE(numEnum.getValue() == 10);
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasInvisibilityFlag));
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasBombFlag));
    REQUIRE(numEnum.hasFlag(tson::TestEnumNumberFlags::HasInvisibilityFlag));
    REQUIRE(!numEnum.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag));

    REQUIRE(c.getMember("StrFlag")->getType() == tson::Type::Enum);
    REQUIRE(strEnum.getValue() == 6);
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strEnum.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!strEnum.hasFlag(tson::TestEnumStringFlags::HasMoneyFlag));

    // Check the same with the copy of class
    // -------------------------------------

    REQUIRE(c2.getId() == 1);
    REQUIRE(c2.getName() == "TestClass");
    REQUIRE(c2.getType() == "class");
    REQUIRE(c2.getMembers().getSize() == 9);

    REQUIRE(c2.getMember("Age")->getType() == tson::Type::Int);
    REQUIRE(c2.getMember("Age")->getValue<int>() == 49);
    REQUIRE(c2.get<int>("Age") == 49);
    REQUIRE(c2.getMember("CanDestroy")->getType() == tson::Type::Boolean);
    REQUIRE(c2.get<bool>("CanDestroy"));
    REQUIRE(c2.getMember("ExtraFile")->getType() == tson::Type::File);
    REQUIRE(c2.get<fs::path>("ExtraFile") == fs::path("../ultimate_test.json"));
    REQUIRE(c2.getMember("MoneyInBag")->getType() == tson::Type::Float);
    REQUIRE(tson::Tools::Equal(c2.get<float>("MoneyInBag"), 16.9344f));
    REQUIRE(c2.getMember("MyObject")->getType() == tson::Type::Object);
    REQUIRE(c2.get<uint32_t>("MyObject") == 39);
    REQUIRE(c2.getMember("Name")->getType() == tson::Type::String);
    REQUIRE(c2.get<std::string>("Name") == "James Testolini");
    REQUIRE(c2.getMember("ShoeColor")->getType() == tson::Type::Color);
    tson::Colori colorC2 = c2.get<tson::Colori>("ShoeColor");
    REQUIRE(colorC2 == "#ff069504");
    REQUIRE(colorC2.a == 0xff);
    REQUIRE(colorC2.r == 0x06);
    REQUIRE(colorC2.g == 0x95);
    REQUIRE(colorC2.b == 0x04);

    tson::EnumValue numEnumC2 = c2.getMember("NumFlag")->getValue<tson::EnumValue>();
    tson::EnumValue strEnumC2 = c2.getMember("StrFlag")->getValue<tson::EnumValue>();

    REQUIRE(c2.getMember("NumFlag")->getType() == tson::Type::Enum);
    REQUIRE(numEnumC2.getValue() == 10);
    REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasInvisibilityFlag)); //Has both these flags - OK
    REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasBombFlag)); // Has this flag - OK
    REQUIRE(numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasInvisibilityFlag)); // Has this flag - OK
    REQUIRE(numEnumC2.hasAnyFlag(tson::TestEnumNumberFlags::HasBombFlag | tson::TestEnumNumberFlags::HasHumorFlag)); //hasAnyFlag is okay as long as one of the flags here are set
    REQUIRE(!numEnumC2.hasFlag(tson::TestEnumNumberFlags::HasHumorFlag)); //Doesn't have this flag - OK

    REQUIRE(c2.getMember("StrFlag")->getType() == tson::Type::Enum);
    REQUIRE(strEnumC2.getValue() == 6);
    REQUIRE(strEnumC2.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strEnumC2.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strEnumC2.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!strEnumC2.hasFlag(tson::TestEnumStringFlags::HasMoneyFlag));
}
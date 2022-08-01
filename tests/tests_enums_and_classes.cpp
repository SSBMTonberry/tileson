//
// Created by robin on 21.07.22.
//

#include "../external_libs/catch.hpp"

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../single_include/tileson.hpp"
#else
    #include "../include/tileson.h"
#endif


namespace tson
{
    enum class TestEnumNumberFlags : uint32_t
    {
        None = 0,
        HasCalculatorFlag = 1 << 0,
        HasBombFlag = 1 << 1,
        HasHumorFlag = 1 << 2,
        HasInvisibilityFlag = 1 << 3,
        All = HasCalculatorFlag | HasBombFlag | HasHumorFlag | HasInvisibilityFlag
    };

    enum class TestEnumStringFlags : uint32_t
    {
        None = 0,
        HasCarFlag = 1 << 0,
        HasJobFlag = 1 << 1,
        HasHouseFlag = 1 << 2,
        HasMoneyFlag = 1 << 3,
        All = HasCarFlag | HasJobFlag | HasHouseFlag | HasMoneyFlag
    };
}

TILESON_ENABLE_BITMASK_OPERATORS(TestEnumNumberFlags)
TILESON_ENABLE_BITMASK_OPERATORS(TestEnumStringFlags)

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
    REQUIRE(strV1.hasFlagValue(1));
    REQUIRE(strV2.hasFlagValue(2));
    REQUIRE(strV3.hasFlagValue(3));
    REQUIRE(strV4.hasFlagValue(4));
    REQUIRE(!strV4.hasFlagValue(97));
    REQUIRE(numV1.getName() == "CreateNumber");
    REQUIRE(numV2.getName() == "DeleteNumber");
    REQUIRE(numV3.getName() == "UpdateNumber");
    REQUIRE(numV4.getName() == "GetNumber");
    REQUIRE(numV1.hasFlagValue(1));
    REQUIRE(numV2.hasFlagValue(2));
    REQUIRE(numV3.hasFlagValue(3));
    REQUIRE(numV4.hasFlagValue(4));
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
    REQUIRE(numV1.getName() == "HasCalculatorFlag");
    REQUIRE(numV2.getName() == "HasBombFlag");
    REQUIRE(numV3.getName() == "HasHumorFlag");
    REQUIRE(numV4.getName() == "HasInvisibilityFlag");

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

    REQUIRE(def.getValue(1) == "CreatePlayer");
    REQUIRE(def.exists(1));
    REQUIRE(def.getValue(2) == "UpdatePlayer");
    REQUIRE(def.exists(2));
    REQUIRE(def.getValue(3) == "DeletePlayer");
    REQUIRE(def.exists(3));
    REQUIRE(def.getValue(4) == "GetPlayer");
    REQUIRE(def.exists(4));
    REQUIRE(def.getValue(97).empty());
    REQUIRE(!def.exists(97));

    REQUIRE(def.getValue("CreatePlayer") == 1);
    REQUIRE(def.exists("CreatePlayer"));
    REQUIRE(def.getValue("UpdatePlayer") == 2);
    REQUIRE(def.exists("UpdatePlayer"));
    REQUIRE(def.getValue("DeletePlayer") == 3);
    REQUIRE(def.exists("DeletePlayer"));
    REQUIRE(def.getValue("GetPlayer") == 4);
    REQUIRE(def.exists("GetPlayer"));
    REQUIRE(def.getValue("None") == 0);
    REQUIRE(def.getValue("SomethingInvalid") == 0);
    REQUIRE(!def.exists("SomethingInvalid"));

    tson::EnumValue strV1 {"CreatePlayer", &def};
    tson::EnumValue strV2 {"DeletePlayer", &def};
    tson::EnumValue strV3 {"UpdatePlayer", &def};
    tson::EnumValue strV4 {"GetPlayer", &def};
    tson::EnumValue numV1 {1, &def};
    tson::EnumValue numV2 {2, &def};
    tson::EnumValue numV3 {3, &def};
    tson::EnumValue numV4 {4, &def};

    REQUIRE(strV1.getValue() == 1);
    REQUIRE(strV2.getValue() == 3);
    REQUIRE(strV3.getValue() == 2);
    REQUIRE(strV4.getValue() == 4);
    REQUIRE(strV1.hasFlagValue(1));
    REQUIRE(strV3.hasFlagValue(2));
    REQUIRE(strV2.hasFlagValue(3));
    REQUIRE(strV4.hasFlagValue(4));
    REQUIRE(!strV4.hasFlagValue(97));
    REQUIRE(numV1.getName() == "CreatePlayer");
    REQUIRE(numV2.getName() == "UpdatePlayer");
    REQUIRE(numV3.getName() == "DeletePlayer");
    REQUIRE(numV4.getName() == "GetPlayer");
    REQUIRE(numV1.hasFlagValue(1));
    REQUIRE(numV2.hasFlagValue(2));
    REQUIRE(numV3.hasFlagValue(3));
    REQUIRE(numV4.hasFlagValue(4));
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
    REQUIRE(numV1.getName() == "HasCarFlag");
    REQUIRE(numV2.getName() == "HasJobFlag");
    REQUIRE(numV3.getName() == "HasHouseFlag");
    REQUIRE(numV4.getName() == "HasMoneyFlag");

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

    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(strFlagV2.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!strFlagV2.hasFlag(tson::TestEnumStringFlags::HasCarFlag));

    REQUIRE(numFlagV1.hasFlag(tson::TestEnumStringFlags::All));

    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag | tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasJobFlag));
    REQUIRE(numFlagV2.hasFlag(tson::TestEnumStringFlags::HasHouseFlag));
    REQUIRE(!numFlagV2.hasFlag(tson::TestEnumStringFlags::HasCarFlag));
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
                              "    \"folders\": [\n"
                              "        \"maps\",\n"
                              "        \"world\"\n"
                              "    ],\n"
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


    REQUIRE(c.getMember("StrFlag")->getType() == tson::Type::Enum);
    REQUIRE(c.getMember("NumFlag")->getType() == tson::Type::Enum);
}
//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/catch.hpp"
#include "../src/Tileson.h"

#include "tson_files_mapper.h"

TEST_CASE( "Parse a whole map by file", "[complete][parse][file]" ) {
    tson::Tileson t;
    tson::Map map = t.parse({"../../content/test-maps/ultimate_test.json"});
    REQUIRE( !map.getLayers().empty() );
}

TEST_CASE( "Parse a whole map by memory", "[complete][parse][memory]" ) {
    tson::Tileson t;
    tson::Map map = t.parse(tson_files::_ULTIMATE_TEST_JSON, tson_files::_ULTIMATE_TEST_JSON_SIZE);
    REQUIRE( !map.getLayers().empty() );
}
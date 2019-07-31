//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/catch.hpp"
#include "../src/TilesonParser.h"

TEST_CASE( "Create parser and do absolutely nothing - expect 'Undefined' status", "[dummy]" ) {
    tson::TilesonParser parser;
    tson::ParserStatus status = parser.getStatus();
    REQUIRE( status == tson::ParserStatus::Undefined);
}
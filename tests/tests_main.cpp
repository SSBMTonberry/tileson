//
// Created by robin on 31.07.2019.
//

#define CATCH_CONFIG_MAIN
#include "../external_libs/catch.hpp"
#include "../src/Tileson.h"

TEST_CASE( "Create parser and do absolutely nothing - expect 'Undefined' status", "[dummy]" ) {
    tson::Tileson t;
    REQUIRE( true );
}
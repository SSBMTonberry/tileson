//
// Created by robin on 31.07.2019.
//

#include "../external_libs/catch.hpp"
#include "../src/objects/Color.h"



TEST_CASE( "Create parser and do absolutely nothing - expect 'Undefined' status", "[dummy]" ) {
    tson::Color color {"#ffaa07ff"};
    REQUIRE( true );
}
//
// Created by robin on 31.07.2019.
//

#include "../external_libs/catch.hpp"
#include "../src/objects/Color.h"
#include <tuple>

TEST_CASE( "Parse a color from Tiled color string with alpha - expect valid color", "[color][argb]" ) {
    tson::Color color {"#ffaa07ff"};
    tson::Color expected {170, 7, 255, 255 };
    REQUIRE( color == expected );
}

TEST_CASE( "Parse a color from Tiled color string without alpha - expect valid color", "[color][rgb]" ) {
    tson::Color color {"#aa07ff"};
    tson::Color expected {170, 7, 255, 255 };
    REQUIRE( color == expected );
}

TEST_CASE( "Parse a color from Tiled color and get as float- expect correct float values", "[color][float]" ) {
    tson::Color color {29, 170, 205, 255};
    std::tuple<float, float, float, float> expected {(float)29 / 255, (float)170 / 255, (float)205 / 255, (float)255 / 255};
    std::tuple<float, float, float, float> result = color.asFloat();
    REQUIRE( result == expected );
}

TEST_CASE( "Compare color and its related hex as sting - expect success", "[color][comparison][string]" ) {
    tson::Color color {170, 7, 255, 255 };
    REQUIRE( color == "#ffaa07ff" );
}
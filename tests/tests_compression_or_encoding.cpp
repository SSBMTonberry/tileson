//
// Created by robin on 30.07.2020.
//

#include "../external_libs/catch.hpp"
#include "tileson.h"

TEST_CASE( "A simple base64-encoded sentence - expect right decoded result", "[base64][decoding]" )
{
    std::string expected = "I have a sentence here that I want to get from a base64";

    std::string base64_str = "SSBoYXZlIGEgc2VudGVuY2UgaGVyZSB0aGF0IEkgd2FudCB0byBnZXQgZnJvbSBhIGJhc2U2NA==";
    tson::Base64Decompressor decompressor;
    std::string result = decompressor.decompress(base64_str);

    REQUIRE(expected == result);
}
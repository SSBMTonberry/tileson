//
// Created by robin on 30.07.2020.
//

#include <utility>

#include "../external_libs/catch.hpp"
#include "tileson.h"

class DummyDecompressor : public tson::IDecompressor
{
    public:
        DummyDecompressor() : m_name {"undef"}, m_result {"undef"}
        {

        }

        DummyDecompressor(std::string name, std::string result)
        : m_name {std::move(name)}, m_result {std::move(result)}
        {

        }

        const std::string &name() const override
        {
            return m_name;
        }

        std::string decompress(std::string_view s) override
        {
            return m_result;
        }

    private:
        std::string m_name;
        std::string m_result;
};

TEST_CASE( "A simple base64-encoded sentence - expect right decoded result", "[base64][decoding]" )
{
    std::string expected = "I have a sentence here that I want to get from a base64";

    std::string base64_str = "SSBoYXZlIGEgc2VudGVuY2UgaGVyZSB0aGF0IEkgd2FudCB0byBnZXQgZnJvbSBhIGJhc2U2NA==";
    tson::Base64Decompressor decompressor;
    std::string result = decompressor.decompress(base64_str);

    REQUIRE(expected == result);
}

TEST_CASE( "DecompressorContainer - Add elements and check if they exist", "[decompress][container]" )
{
    tson::DecompressorContainer container;
    container.add<DummyDecompressor>("base64", "BASERESULT!");

    REQUIRE(container.contains("base64"));
    REQUIRE(!container.contains("something_else"));
}

TEST_CASE( "DecompressorContainer - Add three elements, remove one, expect two left.", "[decompress][container][remove]" )
{
    tson::DecompressorContainer container;
    container.add<DummyDecompressor>("test1", "BASERESULT!");
    container.add<DummyDecompressor>("test2", "BASERESULT!");
    container.add<DummyDecompressor>("test3", "BASERESULT!");

    container.remove("test2");

    REQUIRE(container.contains("test1"));
    REQUIRE(!container.contains("test2"));
    REQUIRE(container.contains("test3"));
    REQUIRE(container.size() == 2);
}
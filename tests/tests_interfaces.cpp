//
// Created by robin on 06.01.2021.
//

#include "../external_libs/catch.hpp"
#include "../include/tileson.h"

class FakeSon : public tson::IJson
{
    public:

        bool parse(const void *data, size_t size) override
        {
            return false;
        }

        [[nodiscard]] size_t count(std::string_view key) const override
        {
            return (key == "test") ? 1 : 0;
        }

        [[nodiscard]] bool any(std::string_view key) const override
        {
            return (key == "test");
        }

        [[nodiscard]] bool isArray() const override
        {
            return false;
        }

        [[nodiscard]] bool isObject() const override
        {
            return true;
        }

        [[nodiscard]] bool isNull() const override
        {
            return false;
        }

    protected:
        [[nodiscard]] int32_t getInt32(std::string_view key) const override
        {
            return (key == "test") ? -32 : -1;
        }

        [[nodiscard]] uint32_t getUInt32(std::string_view key) const override
        {
            return (key == "test") ? 32 : 0;
        }

        [[nodiscard]] int64_t getInt64(std::string_view key) const override
        {
            return (key == "test") ? -64 : -1;
        }

        [[nodiscard]] uint64_t getUInt64(std::string_view key) const override
        {
            return (key == "test") ? 64 : 0;
        }

        [[nodiscard]] double getDouble(std::string_view key) const override
        {
            return (key == "test") ? 1337.1337 : 1.0;
        }

        [[nodiscard]] const std::string &getString(std::string_view key) const override
        {
            return (key == "test") ? m_str : m_str2;
        }

        [[nodiscard]] bool getBool(std::string_view key) const override
        {
            return (key == "test");
        }

    private:
        std::string m_str {"Test"};
        std::string m_str2 {"No value"};
};

//template<typename T>
//T tson::IJson::get(std::string_view key) const
//{
//    static T defaultT;
//    return defaultT;
//}

TEST_CASE( "Test a simple FAKE-implementation of IJson - Expect OK.", "[json][interface]" ) {
    std::unique_ptr<tson::IJson> j = std::make_unique<FakeSon>();
    REQUIRE( j->count("test") == 1 );
    REQUIRE( j->count("best") == 0 );
    REQUIRE( j->any("test"));
    REQUIRE( !j->any("best"));
    REQUIRE( j->isObject());

    REQUIRE( j->get<int32_t>("test") == -32 );
    REQUIRE( j->get<int32_t>("best") == -1 );
    REQUIRE( j->get<uint32_t>("test") == 32 );
    REQUIRE( j->get<uint32_t>("best") == 0 );
    REQUIRE( j->get<int64_t>("test") == -64 );
    REQUIRE( j->get<int64_t>("best") == -1 );
    REQUIRE( j->get<uint64_t>("test") == 64 );
    REQUIRE( j->get<uint64_t>("best") == 0 );
    REQUIRE( j->get<double>("test") > 1337.0 );
    REQUIRE( j->get<double>("best") < 2 );
    REQUIRE( j->get<std::string>("test") == "Test" );
    REQUIRE( j->get<std::string>("best") == "No value" );
    REQUIRE( j->get<bool>("test"));
    REQUIRE( !j->get<bool>("best"));
    REQUIRE( j->get<nullptr_t>("whatever") == nullptr);
}
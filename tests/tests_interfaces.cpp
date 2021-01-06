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
            return 0;
        }

        [[nodiscard]] bool any(std::string_view key) const override
        {
            return false;
        }


};

//template<typename T>
//T tson::IJson::get(std::string_view key) const
//{
//    static T defaultT;
//    return defaultT;
//}

TEST_CASE( "Test a simple implementation of IJson - Expect OK.", "[color][argb]" ) {
    std::unique_ptr<tson::IJson> j = std::make_unique<FakeSon>();
    int i = j->get<int>("Hey, brother");
    REQUIRE( true );
}
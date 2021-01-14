//
// Created by robin on 13.01.2021.
//

#include "../external_libs/catch.hpp"
#include "../include/external/nlohmann.hpp"
#include "../include/tileson.h"

TEST_CASE( "Run benchmarks on PicoJson and NlohmannJson", "[json][benchmarks]" )
{
    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;

    tson::Tileson nlohmann {std::make_unique<tson::NlohmannJson>()};
    tson::Tileson picojson {std::make_unique<tson::PicoJson>()};
    tson::Tileson gason {std::make_unique<tson::Gason>()};

    auto startNlohmann = std::chrono::steady_clock::now();
    auto nlohmannMap = nlohmann.parse(pathToUse);
    auto endNlohmann = std::chrono::steady_clock::now();
    std::chrono::duration<double> msNlohmann = (endNlohmann-startNlohmann) * 1000;

    auto startPicoJson = std::chrono::steady_clock::now();
    auto picojsonMap = picojson.parse(pathToUse);
    auto endPicoJson = std::chrono::steady_clock::now();
    std::chrono::duration<double> msPicoJson = (endPicoJson-startPicoJson) * 1000;

    auto startGason = std::chrono::steady_clock::now();
    auto gasonMap = gason.parse(pathToUse);
    auto endGason = std::chrono::steady_clock::now();
    std::chrono::duration<double> msGason = (endGason-startGason) * 1000;

    std::cout << "Nlohmann parse time: " << msNlohmann.count() << " ms\n";
    std::cout << "PicoJson parse time: " << msPicoJson.count() << " ms\n";
    std::cout << "Gason parse time: " << msGason.count() << " ms\n";

    REQUIRE(nlohmannMap->getStatus() == tson::ParseStatus::OK);
    REQUIRE(picojsonMap->getStatus() == tson::ParseStatus::OK);
    REQUIRE(gasonMap->getStatus() == tson::ParseStatus::OK);
}
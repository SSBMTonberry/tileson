//
// Created by robin on 13.01.2021.
//

#include "../external_libs/catch.hpp"

//#define POCKETLZMA_LZMA_C_DEFINE //Already defined once in tests_main
#include "../extras/pocketlzma.hpp"

#include "../include/external/nlohmann.hpp"
#include "../include/external/picojson.hpp"
#include "../include/external/gason.hpp"
#include "../include/tileson.h"

#include "tson_files_mapper.h"

//Must be defined after Tileson
#include "../include/json/Gason.hpp"
#include "TestTools.hpp"

#include <memory>
#include <map>
#include <functional>

#if __clang_major__ < 10
TEST_CASE( "Run benchmarks on all supported json-backends", "[json][benchmarks]" )
{

    fs::path pathToUse = GetPathWithBase(fs::path("test-maps/ultimate_test.json"));
    fs::path pathLzmaToUse = GetPathWithBase(fs::path("test-maps/ultimate_test.lzma"));

    if(fs::exists(pathToUse) && fs::exists(pathLzmaToUse))
    {
        tson::Tileson nlohmann{std::make_unique<tson::NlohmannJson>()};
        tson::Tileson picojson{std::make_unique<tson::PicoJson>()};
        tson::Tileson j11{std::make_unique<tson::Json11>()};

        auto startNlohmann = std::chrono::steady_clock::now();
        auto nlohmannMap = nlohmann.parse(pathToUse);
        auto endNlohmann = std::chrono::steady_clock::now();
        std::chrono::duration<double> msNlohmann = (endNlohmann - startNlohmann) * 1000;

        auto startPicoJson = std::chrono::steady_clock::now();
        auto picojsonMap = picojson.parse(pathToUse);
        auto endPicoJson = std::chrono::steady_clock::now();
        std::chrono::duration<double> msPicoJson = (endPicoJson - startPicoJson) * 1000;

        auto startJ11 = std::chrono::steady_clock::now();
        auto j11Map = j11.parse(pathToUse);
        auto endJ11 = std::chrono::steady_clock::now();
        std::chrono::duration<double> msJ11 = (endJ11 - startJ11) * 1000;

        auto startJ11Lzma = std::chrono::steady_clock::now();
        auto j11MapLzma = j11.parse(pathLzmaToUse, std::make_unique<tson::Lzma>());
        auto endJ11Lzma = std::chrono::steady_clock::now();
        std::chrono::duration<double> msJ11Lzma = (endJ11Lzma - startJ11Lzma) * 1000;

        auto startJ11LzmaMemory = std::chrono::steady_clock::now();
        std::unique_ptr<tson::Map> j11MapLzmaMemory = j11.parse(tson_files_mapper::_ULTIMATE_TEST_LZMA, tson_files_mapper::_ULTIMATE_TEST_LZMA_SIZE,
                                                                std::make_unique<tson::Lzma>());
        auto endJ11LzmaMemory = std::chrono::steady_clock::now();
        std::chrono::duration<double> msJ11LzmaMemory = (endJ11LzmaMemory - startJ11LzmaMemory) * 1000;

        std::cout << "Nlohmann parse time:             " << msNlohmann.count() << " ms\n";
        std::cout << "PicoJson parse time:             " << msPicoJson.count() << " ms\n";
        std::cout << "Json11 parse time:               " << msJ11.count() << " ms\n";
        std::cout << "Json11 parse time (LZMA):        " << msJ11Lzma.count() << " ms\n";
        std::cout << "Json11 memory parse time (LZMA): " << msJ11LzmaMemory.count() << " ms\n";

        REQUIRE(nlohmannMap->getStatus() == tson::ParseStatus::OK);
        REQUIRE(picojsonMap->getStatus() == tson::ParseStatus::OK);
        REQUIRE(j11Map->getStatus() == tson::ParseStatus::OK);
        REQUIRE(j11MapLzma->getStatus() == tson::ParseStatus::OK);
        REQUIRE(j11MapLzmaMemory->getStatus() == tson::ParseStatus::OK);
    }
    else //If some systems cannot pick up the files, it's really not that important, so just ignore!
    {
        std::cout << "Benchmark test was unable to locate maps. IGNORED! \n";
        REQUIRE(true);
    }
}
#else
TEST_CASE( "Run benchmarks on all supported json-backends", "[json][benchmarks]" )
{
    std::cout << "Ignored - Parsing compressed maps currently does not work on Clang 11 or newer. Must probably be fixed in PocketLzma, and is related to that library. " << std::endl;
    REQUIRE(true);
}
#endif

//
// Removed due to Gason being unsupported and thus unknown errors during parsing are expected
//
//TEST_CASE( "Run benchmarks on Gason (unsupported)", "[json][benchmarks]" )
//{
//    fs::path pathLocal {"../../content/test-maps/ultimate_test.json"};
//    fs::path pathTravis {"../content/test-maps/ultimate_test.json"};
//    fs::path pathToUse = (fs::exists(pathLocal)) ? pathLocal : pathTravis;
//
//    fs::path pathLzmaLocal {"../../content/test-maps/ultimate_test.lzma"};
//    fs::path pathLzmaTravis {"../content/test-maps/ultimate_test.lzma"};
//    fs::path pathLzmaToUse = (fs::exists(pathLzmaLocal)) ? pathLzmaLocal : pathLzmaTravis;
//
//    tson::Tileson g {std::make_unique<tson::Gason>()};
//
//    auto startGason = std::chrono::steady_clock::now();
//    auto gasonMap = g.parse(pathToUse);
//    auto endGason = std::chrono::steady_clock::now();
//    std::chrono::duration<double> msGason = (endGason-startGason) * 1000;
//
//    std::cout << "Gason parse time:                " << msGason.count() << " ms\n";
//
//    REQUIRE(gasonMap->getStatus() == tson::ParseStatus::OK);
//}
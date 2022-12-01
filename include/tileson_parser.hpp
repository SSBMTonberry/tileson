//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESON_PARSER_HPP
#define TILESON_TILESON_PARSER_HPP

//RBP: FS-namespace is defined in tileson_parser now!
#if _MSC_VER && !__INTEL_COMPILER
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __MINGW64__
    #if __MINGW64_VERSION_MAJOR > 6
        #include <filesystem>
        namespace fs = std::filesystem;
    #else
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #endif
#elif __clang__
    #if __clang_major__ < 8
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #include <filesystem>
        namespace fs = std::filesystem;
    #endif
#else //Linux
    #if __GNUC__ < 8 //GCC major version less than 8
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #else
        #include <filesystem>
        namespace fs = std::filesystem;
    #endif
#endif

#include <fstream>
#include <sstream>
#include <memory>

#include "common/Tools.hpp"
#include "common/Base64Decompressor.hpp"
#include "common/Lzma.hpp"
#include "common/DecompressorContainer.hpp"
#include "misc/MemoryStream.hpp"
#include "tiled/Map.hpp"
#include "tiled/TiledEnum.hpp"
#include "tiled/TiledClass.hpp"
#include "tiled/Project.hpp"


namespace tson
{
    class Tileson
    {
        public:
            #ifdef JSON11_IS_DEFINED
            inline explicit Tileson(std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>(), bool includeBase64Decoder = true);
            inline explicit Tileson(tson::Project *project, std::unique_ptr<tson::IJson> jsonParser = std::make_unique<tson::Json11>(), bool includeBase64Decoder = true);
            #else
            inline explicit Tileson(std::unique_ptr<tson::IJson> jsonParser, bool includeBase64Decoder = true);
            inline explicit Tileson(tson::Project *project, std::unique_ptr<tson::IJson> jsonParser, bool includeBase64Decoder = true);
            #endif

            inline std::unique_ptr<tson::Map> parse(const fs::path &path, std::unique_ptr<IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor = nullptr);
            inline std::unique_ptr<tson::Map> parse(const void * data, size_t size, std::unique_ptr<IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor = nullptr);
            inline tson::DecompressorContainer *decompressors();

        private:
            inline std::unique_ptr<tson::Map> parseJson();
            std::unique_ptr<tson::IJson> m_json;
            tson::DecompressorContainer m_decompressors;
            tson::Project *m_project {nullptr};
    };
}

/*!
 *
 * @param includeBase64Decoder Includes the base64-decoder from "Base64Decompressor.hpp" if true.
 * Otherwise no other decompressors/decoders than whatever the user itself have added will be used.
 */
tson::Tileson::Tileson(std::unique_ptr<tson::IJson> jsonParser, bool includeBase64Decoder) : m_json {std::move(jsonParser)}
{
    if(includeBase64Decoder)
        m_decompressors.add<Base64Decompressor>();
}

tson::Tileson::Tileson(tson::Project *project, std::unique_ptr<tson::IJson> jsonParser, bool includeBase64Decoder) : m_json {std::move(jsonParser)}
{
    m_project = project;
    if(includeBase64Decoder)
        m_decompressors.add<Base64Decompressor>();
}

/*!
 * Parses Tiled json map data by file
 * @param path path to file
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parse(const fs::path &path, std::unique_ptr<IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor)
{

    bool result = false;

    if(decompressor != nullptr)
    {
        std::vector<uint8_t> decompressed = decompressor->decompressFile(path);
        result = (decompressed.empty()) ? false : true;
        if(!result)
            return std::make_unique<tson::Map>(tson::ParseStatus::DecompressionError, "Error during decompression");

        result = m_json->parse(&decompressed[0], decompressed.size());

        if(result)
            return parseJson();
    }
    else if(m_json->parse(path))
    {
        return parseJson();
    }

    std::string msg = "File not found: ";
    msg += std::string(path.generic_string());
    return std::make_unique<tson::Map>(tson::ParseStatus::FileNotFound, msg);
}

/*!
 * Parses Tiled json map data by memory
 * @param data The data to parse
 * @param size The size of the data to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parse(const void *data, size_t size, std::unique_ptr<IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor)
{
    bool result = false;

    if(decompressor != nullptr)
    {
        std::vector<uint8_t> decompressed = decompressor->decompress(data, size);
        result = (decompressed.empty()) ? false : true;
        if(!result)
            return std::make_unique<tson::Map>(tson::ParseStatus::DecompressionError, "Error during decompression");
        result = m_json->parse(&decompressed[0], decompressed.size());
    }
    else
        result = m_json->parse(data, size);

    if(!result)
        return std::make_unique<tson::Map>(tson::ParseStatus::ParseError, "Memory error");


    return parseJson();
}

/*!
 * Common parsing functionality for doing the json parsing
 * @param json Tiled json to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parseJson()
{
    std::unique_ptr<tson::Map> map = std::make_unique<tson::Map>();

    if(map->parse(*m_json, &m_decompressors, m_project))
        return map;

    return std::make_unique<tson::Map> (tson::ParseStatus::MissingData, "Missing map data...");
}

/*!
 * Gets the decompressor container used when something is either encoded or compressed (regardless: IDecompressor is used as base).
 * These are used specifically for tile layers, and are connected by checking the name of the IDecompressor. If the name of a decompressor
 * matches with an encoding or a compression, its decompress() function will be used.
 *
 * @return The container including all decompressors.
 */
tson::DecompressorContainer *tson::Tileson::decompressors()
{
    return &m_decompressors;
}

#endif //TILESON_TILESON_PARSER_HPP

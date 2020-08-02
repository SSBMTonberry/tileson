//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESON_PARSER_HPP
#define TILESON_TILESON_PARSER_HPP

#include "common/Tools.hpp"
#include "common/Base64Decompressor.hpp"
#include "common/DecompressorContainer.hpp"
#include "tiled/Map.hpp"
#include "tiled/Project.hpp"
#include "misc/MemoryStream.hpp"
#include <fstream>
#include <sstream>
#include <memory>

namespace tson
{
    class Tileson
    {
        public:
            inline explicit Tileson(bool includeBase64Decoder = true);
            #ifndef DISABLE_CPP17_FILESYSTEM
            inline std::unique_ptr<tson::Map> parse(const fs::path &path);
            #else
            inline std::unique_ptr<tson::Map> parse(const std::string &path);
            #endif
            inline std::unique_ptr<tson::Map> parse(const void * data, size_t size);
            inline tson::DecompressorContainer *decompressors();
        private:
            inline std::unique_ptr<tson::Map> parseJson(const nlohmann::json &json);
            tson::DecompressorContainer m_decompressors;
    };
}

/*!
 *
 * @param includeBase64Decoder Includes the base64-decoder from "Base64Decompressor.hpp" if true.
 * Otherwise no other decompressors/decoders than whatever the user itself have added will be used.
 */
tson::Tileson::Tileson(bool includeBase64Decoder)
{
    if(includeBase64Decoder)
        m_decompressors.add<Base64Decompressor>();
}

/*!
 * Parses Tiled json map data by file
 * @param path path to file
 * @return parsed data as Map
 */
#ifndef DISABLE_CPP17_FILESYSTEM
std::unique_ptr<tson::Map> tson::Tileson::parse(const fs::path &path)
{
    if(fs::exists(path) && fs::is_regular_file(path))
    {
        std::ifstream i(path.u8string());
        nlohmann::json json;
        try
        {
            i >> json;
        }
        catch(const nlohmann::json::parse_error &error)
        {
            std::string message = "Parse error: ";
            message += std::string(error.what());
            message += std::string("\n");
            return std::make_unique<tson::Map>(tson::ParseStatus::ParseError, message);
        }
        return parseJson(json);
    }

    std::string msg = "File not found: ";
    msg += std::string(path.u8string());
    return std::make_unique<tson::Map>(tson::ParseStatus::FileNotFound, msg);
}
#else
[[deprecated("std::filesystem will be required in future versions and DISABLE_CPP17_FILESYSTEM will be removed")]]
std::unique_ptr<tson::Map> tson::Tileson::parse(const std::string &path)
{

    std::ifstream i(path);
    nlohmann::json json;
    try
    {
        i >> json;
    }
    catch(const nlohmann::json::parse_error &error)
    {
        std::string message = "Parse error: ";
        message += std::string(error.what());
        message += std::string("\n");
        return std::make_unique<tson::Map> (tson::ParseStatus::ParseError, message);
    }
    return std::move(parseJson(json));
}
#endif
/*!
 * Parses Tiled json map data by memory
 * @param data The data to parse
 * @param size The size of the data to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parse(const void *data, size_t size)
{
    //std::istringstream i;
    //i.rdbuf()->pubsetbuf((char *)data, size);

    tson::MemoryStream mem {(uint8_t *)data, size};

    nlohmann::json json;
    try
    {
        mem >> json;
    }
    catch (const nlohmann::json::parse_error& error)
    {
        std::string message = "Parse error: ";
        message += std::string(error.what());
        message += std::string("\n");
        return std::make_unique<tson::Map>(tson::ParseStatus::ParseError, message);
    }

    return std::move(parseJson(json));
}

/*!
 * Common parsing functionality for doing the json parsing
 * @param json Tiled json to parse
 * @return parsed data as Map
 */
std::unique_ptr<tson::Map> tson::Tileson::parseJson(const nlohmann::json &json)
{
    std::unique_ptr<tson::Map> map = std::make_unique<tson::Map>();
    if(map->parse(json, &m_decompressors))
        return std::move(map);

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

//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_TILESON_PARSER_HPP
#define TILESON_TILESON_PARSER_HPP

#include "tiled/Map.hpp"
#include "misc/MemoryStream.hpp"
#include <fstream>
#include <sstream>


namespace tson
{
    class Tileson
    {
        public:
            Tileson() = default;
            #if USE_CPP17_FILESYSTEM
            inline tson::Map parse(const fs::path &path);
            #else
            inline tson::Map parse(const std::string &path);
            #endif
            inline tson::Map parse(const void * data, size_t size);

        private:
            inline tson::Map parseJson(const nlohmann::json &json);
    };
}

/*!
 * Parses Tiled json data by file
 * @param path path to file
 * @return parsed data as Map
 */
#if USE_CPP17_FILESYSTEM
tson::Map tson::Tileson::parse(const fs::path &path)
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
            return tson::Map {tson::Map::ParseStatus::ParseError, message};
        }
        return parseJson(json);
    }

    std::string msg = "File not found: ";
    msg += std::string(path.u8string());
    return tson::Map {tson::Map::ParseStatus::FileNotFound, msg};
}
#else
tson::Map tson::Tileson::parse(const std::string &path)
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
        return tson::Map {tson::Map::ParseStatus::ParseError, message};
    }
    return parseJson(json);

}
#endif
/*!
 * Parses Tiled json data by memory
 * @param data The data to parse
 * @param size The size of the data to parse
 * @return parsed data as Map
 */
tson::Map tson::Tileson::parse(const void *data, size_t size)
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
        return tson::Map{ tson::Map::ParseStatus::ParseError, message };
    }

    return parseJson(json);
}

/*!
 * Common parsing functionality for doing the json parsing
 * @param json Tiled json to parse
 * @return parsed data as Map
 */
tson::Map tson::Tileson::parseJson(const nlohmann::json &json)
{
    tson::Map map;
    if(map.parse(json))
        return map;

    return tson::Map {tson::Map::ParseStatus::MissingData, "Missing map data..."};
}

#endif //TILESON_TILESON_PARSER_HPP

//
// Created by robin on 31.07.2019.
//


#include "Tileson.h"

tson::Map tson::Tileson::parse(const fs::path &path)
{
    if(fs::exists(path) && fs::is_regular_file(path))
    {
        std::ifstream i(path.u8string());
        nlohmann::json json;
        i >> json;
        return parseJson(json);
    }

    return tson::Map {};
}

tson::Map tson::Tileson::parse(const void *data, size_t size)
{
    std::istringstream i;
    i.rdbuf()->pubsetbuf((char *)data, size);
    nlohmann::json json;
    i >> json;

    return parseJson(json);
}

tson::Map tson::Tileson::parseJson(const nlohmann::json &json)
{
    tson::Map map;
    if(map.parse(json))
        return map;

    return tson::Map {};
}



//
// Created by robin on 03.08.2019.
//

#include "WangSet.h"

tson::WangSet::WangSet(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangSet::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;
    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;

    return allFound;
}

const std::string &tson::WangSet::getName() const
{
    return m_name;
}

int tson::WangSet::getTile() const
{
    return m_tile;
}

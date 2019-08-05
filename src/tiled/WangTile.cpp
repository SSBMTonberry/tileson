//
// Created by robin on 03.08.2019.
//

#include "WangTile.h"

tson::WangTile::WangTile(const nlohmann::json &json)
{
    parse(json);
}

bool tson::WangTile::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("dflip") > 0) m_dflip = json["dflip"].get<bool>(); else allFound = false;
    if(json.count("hflip") > 0) m_hflip = json["hflip"].get<bool>(); else allFound = false;
    if(json.count("vflip") > 0) m_vflip = json["vflip"].get<bool>(); else allFound = false;
    if(json.count("tileid") > 0) m_tileid = json["tileid"].get<int>(); else allFound = false;

    if(json.count("m_wangid") > 0 && json["m_wangid"].is_array())
        std::for_each(json["m_wangid"].begin(), json["m_wangid"].end(), [&](const nlohmann::json &item) { m_wangId.emplace_back(item.get<int>()); });

    return allFound;
}

bool tson::WangTile::hasDFlip() const
{
    return m_dflip;
}

bool tson::WangTile::hasHFlip() const
{
    return m_hflip;
}

int tson::WangTile::getTileid() const
{
    return m_tileid;
}

bool tson::WangTile::hasVFlip() const
{
    return m_vflip;
}

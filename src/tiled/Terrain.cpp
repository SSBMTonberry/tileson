//
// Created by robin on 03.08.2019.
//

#include "Terrain.h"

tson::Terrain::Terrain(std::string name, int tile) : m_name {std::move(name)}, m_tile {tile}
{

}

tson::Terrain::Terrain(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Terrain::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("name") > 0) m_name = json["name"].get<std::string>(); else allFound = false;
    if(json.count("tile") > 0) m_tile = json["tile"].get<int>(); else allFound = false;

    return allFound;
}

const std::string &tson::Terrain::getName() const
{
    return m_name;
}

int tson::Terrain::getTile() const
{
    return m_tile;
}

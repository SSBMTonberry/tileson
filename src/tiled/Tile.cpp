//
// Created by robin on 03.08.2019.
//

#include "Tile.h"


tson::Tile::Tile(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Tile::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("image") > 0) m_image = fs::path(json["image"].get<std::string>()); //Optional
    if(json.count("id") > 0) m_id = json["id"].get<int>(); else allFound = false;
    if(json.count("type") > 0) m_type = json["type"].get<std::string>(); //Optional

    if(json.count("imagewidth") > 0 && json.count("imageheight") > 0)
        m_imageSize = {json["imagewidth"].get<int>(), json["imageheight"].get<int>()}; //Optional

    return allFound;
}

int tson::Tile::getId() const
{
    return m_id;
}

const fs::path &tson::Tile::getImage() const
{
    return m_image;
}

const tson::Vector2i &tson::Tile::getImageSize() const
{
    return m_imageSize;
}

const std::string &tson::Tile::getType() const
{
    return m_type;
}

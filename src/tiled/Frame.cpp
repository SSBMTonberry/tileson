//
// Created by robin on 03.08.2019.
//

#include "Frame.h"

tson::Frame::Frame(int duration, int tileId) : m_duration {duration}, m_tileId {tileId}
{

}

tson::Frame::Frame(const nlohmann::json &json)
{
    parse(json);
}

bool tson::Frame::parse(const nlohmann::json &json)
{
    bool allFound = true;

    if(json.count("duration") > 0) m_duration = json["duration"].get<int>(); else allFound = false;
    if(json.count("tileid") > 0) m_tileId = json["tileid"].get<int>(); else allFound = false;

    return allFound;
}

int tson::Frame::getDuration() const
{
    return m_duration;
}

int tson::Frame::getTileId() const
{
    return m_tileId;
}

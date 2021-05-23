//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_FRAME_HPP
#define TILESON_FRAME_HPP

//#include "../external/json.hpp"

namespace tson
{
    class Frame
    {
        public:
            inline Frame() = default;
            inline Frame(int duration, uint32_t tileId);
            inline explicit Frame(IJson &json);

            inline bool parse(IJson &json);

            [[nodiscard]] inline int getDuration() const;
            [[nodiscard]] inline uint32_t getTileId() const;

        private:
            int m_duration {};  /*! 'duration': Frame duration in milliseconds */
            uint32_t m_tileId {};    /*! 'tileid': Local tile ID representing this frame */
    };
}

/*!
 *
 * @param duration duration in milliseconds
 * @param tileId TileId
 */
tson::Frame::Frame(int duration, uint32_t tileId) : m_duration {duration}, m_tileId {tileId}
{

}

/*!
 * Parses frame data from json
 * @param json
 */
tson::Frame::Frame(IJson &json)
{
    parse(json);
}

/*!
 * Parses frame data from json
 * @param json
 * @return true if all mandatory fields was found. false otherwise.
 */
bool tson::Frame::parse(IJson &json)
{
    bool allFound = true;

    if(json.count("duration") > 0) m_duration = json["duration"].get<int>(); else allFound = false;
    if(json.count("tileid") > 0) m_tileId = json["tileid"].get<uint32_t>() + 1; else allFound = false;

    return allFound;
}

/*!
 * 'duration': Frame duration in milliseconds
 * @return Duration in milliseconds
 */
int tson::Frame::getDuration() const
{
    return m_duration;
}

/*!
 * 'tileid': Local tile ID representing this frame
 * @return tile id
 */
uint32_t tson::Frame::getTileId() const
{
    return m_tileId;
}

#endif //TILESON_FRAME_HPP

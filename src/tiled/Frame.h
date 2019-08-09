//
// Created by robin on 03.08.2019.
//

#ifndef TILESON_FRAME_H
#define TILESON_FRAME_H

#include <json.hpp>

namespace tson
{
    class Frame
    {
        public:
            Frame() = default;
            Frame(int duration, int tileId);
            Frame(const nlohmann::json &json);

            bool parse(const nlohmann::json &json);

            [[nodiscard]] int getDuration() const;
            [[nodiscard]] int getTileId() const;

        private:
            int m_duration {};  /*! 'duration': Frame duration in milliseconds */
            int m_tileId {};    /*! 'tileid': Local tile ID representing this frame */
    };
}

#endif //TILESON_FRAME_H

//
// Created by robin on 21.05.2021.
//

#ifndef TILESON_ANIMATION_HPP
#define TILESON_ANIMATION_HPP

namespace tson
{
    class Animation
    {
        public:
            inline Animation() = default;
            inline Animation(const std::vector<tson::Frame> &frames) : m_frames {frames} {};

            inline void update(int timeDeltaMs);
            inline const std::vector<tson::Frame> &getFrames() const;
            inline void reset();
            inline const tson::Frame *getCurrentFrame() const;
            inline uint32_t getCurrentFrameNumber() const;
            inline uint32_t getCurrentTileId() const;
            inline int getTimeDelta() const;
            inline size_t size() const;

        private:
            inline int nextFrame();
            std::vector<tson::Frame> m_frames;
            uint32_t m_currentFrame {0};
            int m_timeDelta {0};
    };

    const std::vector<tson::Frame> &Animation::getFrames() const
    {
        return m_frames;
    }

    void Animation::reset()
    {
        m_currentFrame = 0;
        m_timeDelta = 0;
    }

    const tson::Frame *Animation::getCurrentFrame() const
    {
        return (m_frames.size() == 0 || m_currentFrame >= m_frames.size()) ? nullptr : &m_frames.at(m_currentFrame);
    }

    size_t Animation::size() const
    {
        return m_frames.size();
    }

    /*!
     * Update animation based on the fra
     * @param timedeltaMs Time in milliseconds
     */
    void Animation::update(int timeDeltaMs)
    {
        const tson::Frame *frame = getCurrentFrame();
        if(frame != nullptr)
        {
            m_timeDelta += timeDeltaMs;
            if(m_timeDelta >= frame->getDuration())
            {
                m_timeDelta = m_timeDelta % frame->getDuration();
                m_currentFrame = nextFrame();
            }
        }
    }

    int Animation::nextFrame()
    {
        return (m_currentFrame+1 >= m_frames.size()) ? 0 : m_currentFrame + 1;
    }

    int Animation::getTimeDelta() const
    {
        return m_timeDelta;
    }

    uint32_t Animation::getCurrentFrameNumber() const
    {
        return m_currentFrame;
    }

    uint32_t Animation::getCurrentTileId() const
    {
        return (getCurrentFrame() != nullptr) ? getCurrentFrame()->getTileId() : 0;
    }
}

#endif //TILESON_ANIMATION_HPP

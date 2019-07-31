//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_COLOR_H
#define TILESON_COLOR_H

#include <cstdint>
#include <tuple>

namespace tson
{
    class Color
    {
        public:
            Color() = default;
            explicit Color(const std::string &color);
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            [[nodiscard]] uint8_t getRed() const;
            [[nodiscard]] uint8_t getGreen() const;
            [[nodiscard]] uint8_t getBlue() const;
            [[nodiscard]] uint8_t getAlpha() const;

            std::tuple<float, float, float, float> asFloat();
        private:
            void parseHexString(const std::string &color);
            uint8_t m_red = 0;
            uint8_t m_green = 0;
            uint8_t m_blue = 0;
            uint8_t m_alpha = 255;
    };
}

#endif //TILESON_COLOR_H

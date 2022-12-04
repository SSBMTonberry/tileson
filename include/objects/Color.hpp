//
// Created by robin on 09.08.2019.
//

#ifndef TILESON_COLOR_HPP
#define TILESON_COLOR_HPP

#include <type_traits>
#include <cstdint>
#include <string>

namespace tson
{

    template<typename T>
    class Color
    {

        public:
            /*!
             * Parses color from Tiled's own color format, which is #aarrggbb in hex format or optionally #rrggbb.
             * @param color Color in "#rrggbbaa" hex format.
             * @example "#ffaa07ff" and "#aa07ff". In cases where alpha is not a value, it is set to 255.
            */
            inline explicit Color(const std::string &color)
            {
                parseHexString(color);
            }
            inline Color(T red, T green, T blue, T alpha);
            inline Color() { r = g = b = 0; a = 255; }

            inline bool operator==(const Color &rhs) const;
            inline bool operator==(const std::string &rhs) const;
            inline bool operator!=(const Color &rhs) const;

            inline Color<float> asFloat();
            inline Color<uint8_t> asInt();

            /*! Red */
            T r;
            /*! Green */
            T g;
            /*! Blue */
            T b;
            /*! Alpha */
            T a;

        private:
            void parseHexString(const std::string &color)
            {
                if constexpr (std::is_same<T, float>::value)
                {
                    if (color.size() == 9)
                    {
                        a = (float) std::stoi(color.substr(1, 2), nullptr, 16) / 255;
                        r = (float) std::stoi(color.substr(3, 2), nullptr, 16) / 255;
                        g = (float) std::stoi(color.substr(5, 2), nullptr, 16) / 255;
                        b = (float) std::stoi(color.substr(7, 2), nullptr, 16) / 255;
                    }
                    else if (color.size() == 7)
                    {
                        r = (float) std::stoi(color.substr(1, 2), nullptr, 16) / 255;
                        g = (float) std::stoi(color.substr(3, 2), nullptr, 16) / 255;
                        b = (float) std::stoi(color.substr(5, 2), nullptr, 16) / 255;
                        a = 1.f;
                    }
                }
                else if constexpr (std::is_same<T, uint8_t>::value)
                {
                    if (color.size() == 9)
                    {
                        a = static_cast<uint8_t>(std::stoi(color.substr(1, 2), nullptr, 16));
                        r = static_cast<uint8_t>(std::stoi(color.substr(3, 2), nullptr, 16));
                        g = static_cast<uint8_t>(std::stoi(color.substr(5, 2), nullptr, 16));
                        b = static_cast<uint8_t>(std::stoi(color.substr(7, 2), nullptr, 16));
                    }
                    else if (color.size() == 7)
                    {
                        r = static_cast<uint8_t>(std::stoi(color.substr(1, 2), nullptr, 16));
                        g = static_cast<uint8_t>(std::stoi(color.substr(3, 2), nullptr, 16));
                        b = static_cast<uint8_t>(std::stoi(color.substr(5, 2), nullptr, 16));
                        a = 255;
                    }
                }
            }

    };

    typedef Color<uint8_t> Colori;
    typedef Color<float> Colorf;

    /*!
     * Gets the Color as a float. Only useful if the template related to the current color is NOT float
     * @tparam T The template type
     * @return If the T type is float, the value will be returned as a copy of itself. Else: All values will be divided by 255
     * before returning.
     */
    template<typename T>
    tson::Colorf Color<T>::asFloat()
    {
        if constexpr (std::is_same<T, float>::value)
            *this;
        else
            return tson::Colorf((float) r / 255, (float) g / 255, (float) b / 255, (float) a / 255);
    }

    /*!
     * Gets the Color as an 32-bit variable, where each channel is 8-bit.
     * Only useful if the template related to the current color is NOT already 8-bit int
     * @tparam T The template type
     * @return If the T type is float, the value of each channel will be multiplied by 255. Else: The value will be returned as a copy of itself.
     */
    template<typename T>
    tson::Colori Color<T>::asInt()
    {
        if constexpr (std::is_same<T, float>::value)
            return tson::Colori(static_cast<std::uint8_t>((float) r * 255),
                              static_cast<std::uint8_t>((float) g * 255),
                              static_cast<std::uint8_t>((float) b * 255),
                              static_cast<std::uint8_t>((float) a * 255));
        else
            *this;
    }

    /*!
     * Create a new color in rgba (red, green, blue, alpha) format
     * @tparam T the template type for each channel. Usually uint8_t (8-bit int) or float.
     * @param red Red channel
     * @param green Green channel
     * @param blue Blue channel
     * @param alpha Alpha channel
     */
    template<typename T>
    Color<T>::Color(T red, T green, T blue, T alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    template<typename T>
    bool Color<T>::operator==(const std::string &rhs) const {
        Color other {rhs};
        return *this == other;
    }

    template<typename T>
    bool Color<T>::operator==(const Color &rhs) const
    {
        return r == rhs.r &&
               g == rhs.g &&
               b == rhs.b &&
               a == rhs.a;
    }

    template<typename T>
    bool Color<T>::operator!=(const Color &rhs) const
    {
        return !(rhs == *this);
    }




}

#endif //TILESON_COLOR_HPP

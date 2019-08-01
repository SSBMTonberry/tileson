//
// Created by robin on 31.07.2019.
//

#include "Color.h"

/*!
 * Parses color from Tiled's own color format, which is #aarrggbb in hex format or optionally #rrggbb.
 * Example: "#ffaa07ff" and "#aa07ff". In cases where alpha is not a value, it is set to 255.
 * @param color Color in "#rrggbbaa" hex format.
 */
tson::Color::Color(const std::string &color)
{
    parseHexString(color);
}

/*!
 * Create a new color in rgba (red, green, blue, alpha) format
 * @param red Red
 * @param green Green
 * @param blue Blue
 * @param alpha Alpha
 */
tson::Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_alpha = alpha;
}

bool tson::Color::operator==(const tson::Color &rhs) const
{
    return m_red == rhs.m_red &&
           m_green == rhs.m_green &&
           m_blue == rhs.m_blue &&
           m_alpha == rhs.m_alpha;
}

bool tson::Color::operator!=(const tson::Color &rhs) const
{
    return !(rhs == *this);
}

bool tson::Color::operator==(const std::string &rhs) const {
    Color other {rhs};
    return *this == other;
}

/*!
 * @return The amount of red from 0 to 255
 */
uint8_t tson::Color::getRed() const
{
    return m_red;
}

/*!
 * @return The amount of green from 0 to 255
 */
uint8_t tson::Color::getGreen() const
{
    return m_green;
}

/*!
 * @return The amount of blue from 0 to 255
 */
uint8_t tson::Color::getBlue() const
{
    return m_blue;
}

/*!
 * @return The amount of alpha from 0 to 255
 */
uint8_t tson::Color::getAlpha() const
{
    return m_alpha;
}

/*!
 * Gets the rgba value of the color as float. Max value of each channel: 1.f.
 * @return All colors in order: red, green, blue, alpha
 */
std::tuple<float, float, float, float> tson::Color::asFloat()
{
    return std::tuple<float, float, float, float>((float) m_red / 255, (float) m_green / 255, (float) m_blue / 255, (float) m_alpha / 255);
}

void tson::Color::parseHexString(const std::string &color)
{
    if(color.size() == 9)
    {
        m_alpha = std::stoi(color.substr(1, 2), nullptr, 16);
        m_red = std::stoi(color.substr(3, 2), nullptr, 16);
        m_green = std::stoi(color.substr(5, 2), nullptr, 16);
        m_blue = std::stoi(color.substr(7, 2), nullptr, 16);
    }
    else if(color.size() == 7)
    {
        m_red = std::stoi(color.substr(1, 2), nullptr, 16);
        m_green = std::stoi(color.substr(3, 2), nullptr, 16);
        m_blue = std::stoi(color.substr(5, 2), nullptr, 16);
    }
}

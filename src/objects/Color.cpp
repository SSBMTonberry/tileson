//
// Created by robin on 31.07.2019.
//

#include "Color.h"

/*!
 * Parses color from Tiled's own color format, which is #rrggbbaa in hex format. Example: "#ffaa07ff"
 * @param color Color in "#rrggbbaa" hex format.
 */
tson::Color::Color(const std::string &color)
{
    parseHexString(color);
}

/*!
 * Create a new color in rgba (red, green, blue, alpha) format
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Alpha
 */

tson::Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_red = r;
    m_green = g;
    m_blue = b;
    m_alpha = a;
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
        std::string r = color.substr(1, 2);
        std::string g = color.substr(3, 2);
        std::string b = color.substr(5, 2);
        std::string a = color.substr(7, 2);
        size_t red = std::stoi(r);
        size_t green = std::stoi(g);
        size_t blue = std::stoi(b);
        size_t alpha = std::stoi(a);

    }
}

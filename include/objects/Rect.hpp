//
// Created by robin on 24.07.2020.
//

#ifndef TILESON_RECT_HPP
#define TILESON_RECT_HPP

namespace tson
{
    class Rect
    {
        public:

            inline Rect() = default;
            inline Rect(int x_, int y_, int width_, int height_);

            inline bool operator==(const Rect &rhs) const;
            inline bool operator!=(const Rect &rhs) const;

            int x{};
            int y{};
            int width{};
            int height{};
    };

    Rect::Rect(int x_, int y_, int width_, int height_)
    {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }

    bool Rect::operator==(const Rect &rhs) const
    {
        return x == rhs.x &&
               y == rhs.y &&
               width == rhs.width &&
               height == rhs.height;
    }

    bool Rect::operator!=(const Rect &rhs) const
    {
        return !(rhs == *this);
    }
}

#endif //TILESON_RECT_HPP

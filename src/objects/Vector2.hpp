//
// Created by robin on 31.07.2019.
//

#ifndef TILESON_VECTOR2_HPP
#define TILESON_VECTOR2_HPP

namespace tson
{
    template<typename T>
    class Vector2
    {

        public:
            Vector2(T xPos, T yPos);
            Vector2() { x = y = 0; }

            bool operator==(const Vector2 &rhs) const;
            bool operator!=(const Vector2 &rhs) const;

            T x;
            T y;
    };

    /*!
     *
     * @tparam T template type
     * @param xPos x-position
     * @param yPos y-position
     */
    template<typename T>
    Vector2<T>::Vector2(T xPos, T yPos)
    {
        x = xPos;
        y = yPos;
    }

    template<typename T>
    bool Vector2<T>::operator==(const Vector2 &rhs) const
    {
        return x == rhs.x &&
               y == rhs.y;
    }

    template<typename T>
    bool Vector2<T>::operator!=(const Vector2 &rhs) const
    {
        return !(rhs == *this);
    }

    typedef Vector2<int> Vector2i;
    typedef Vector2<float> Vector2f;
}

#endif //TILESON_VECTOR2_HPP

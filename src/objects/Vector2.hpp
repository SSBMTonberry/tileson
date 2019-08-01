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
            Vector2(T x, T y);

            Vector2() { m_x = m_y = 0; }

            bool operator==(const Vector2 &rhs) const;

            bool operator!=(const Vector2 &rhs) const;

        private:
            T m_x;
            T m_y;
    };

    template<typename T>
    Vector2<T>::Vector2(T x, T y)
    {
        m_x = x;
        m_y = y;
    }

    template<typename T>
    bool Vector2<T>::operator==(const Vector2 &rhs) const
    {
        return m_x == rhs.m_x &&
               m_y == rhs.m_y;
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

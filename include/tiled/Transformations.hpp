//
// Created by robin on 04.04.2021.
//

#ifndef TILESON_TRANSFORMATIONS_HPP
#define TILESON_TRANSFORMATIONS_HPP

namespace tson
{
    class Transformations
    {
        public:
            inline Transformations() = default;
            inline explicit Transformations(IJson &json);
            inline bool parse(IJson &json);

            inline bool allowHflip() const;
            inline bool allowPreferuntransformed() const;
            inline bool allowRotation() const;
            inline bool allowVflip() const;

        private:
            bool m_hflip {};                /*! hflip: Whether the tiles in this set can be flipped horizontally (default false) */
            bool m_preferuntransformed {};  /*! preferuntransformed: Whether untransformed tiles remain preferred, otherwise transformed tiles are used to produce more variations (default false) */
            bool m_rotate {};               /*! rotate: Whether the tiles in this set can be rotated in 90 degree increments (default false) */
            bool m_vflip {};                /*! vflip: Whether the tiles in this set can be flipped vertically (default false) */
    };

    Transformations::Transformations(IJson &json)
    {
        parse(json);
    }

    bool Transformations::parse(IJson &json)
    {
        if(json.count("hflip") > 0) m_hflip = json["hflip"].get<bool>(); //Optional
        if(json.count("preferuntransformed") > 0) m_preferuntransformed = json["preferuntransformed"].get<bool>(); //Optional
        if(json.count("rotate") > 0) m_rotate = json["rotate"].get<bool>(); //Optional
        if(json.count("vflip") > 0) m_vflip = json["vflip"].get<bool>(); //Optional

        return true;
    }

    /*!
     *
     * @return hflip: Whether the tiles in this set can be flipped horizontally (default false)
     */
    bool Transformations::allowHflip() const
    {
        return m_hflip;
    }

    /*!
     *
     * @return preferuntransformed: Whether untransformed tiles remain preferred, otherwise transformed tiles are used to produce more variations (default false)
     */
    bool Transformations::allowPreferuntransformed() const
    {
        return m_preferuntransformed;
    }

    /*!
     *
     * @return rotate: Whether the tiles in this set can be rotated in 90 degree increments (default false)
     */
    bool Transformations::allowRotation() const
    {
        return m_rotate;
    }

    /*!
     *
     * @return vflip: Whether the tiles in this set can be flipped vertically (default false)
     */
    bool Transformations::allowVflip() const
    {
        return m_vflip;
    }
}

#endif //TILESON_TRANSFORMATIONS_HPP

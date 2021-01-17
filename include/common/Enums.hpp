//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_ENUMS_HPP
#define TILESON_ENUMS_HPP
#include <cstdint>
#include "EnumBitflags.hpp"

namespace tson
{
    /*!
     * Type used in Property.hpp
     */
    enum class Type : uint8_t
    {
            Undefined = 0,
            Color = 1, /*! color */
            File = 2, /*! file */
            Int = 3, /*! int */
            Boolean = 4, /*! bool */
            Float = 5, /*! float */
            String = 6 /*! string */
    };

    /*!
     * Layer.hpp - LayerType
     * //'type': tilelayer, objectgroup, imagelayer or group
     */
    enum class LayerType : uint8_t
    {
            Undefined = 0,
            TileLayer = 1,
            ObjectGroup = 2,
            ImageLayer = 3,
            Group = 4
    };

    /*!
     * Map.hpp - ParseStatus
     */
    enum class ParseStatus : uint8_t
    {
            OK = 0, //OK unless otherwise stated
            FileNotFound = 1,
            ParseError = 2,
            MissingData = 3,
            DecompressionError = 4
    };

    /*!
     * Object.hpp - ObjectType
     */
    enum class ObjectType : uint8_t
    {
            Undefined = 0,
            Object = 1,
            Ellipse = 2,
            Rectangle = 3,
            Point = 4,
            Polygon = 5,
            Polyline = 6,
            Text = 7,
            Template = 8
    };

    static constexpr uint32_t FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
    static constexpr uint32_t FLIPPED_VERTICALLY_FLAG   = 0x40000000;
    static constexpr uint32_t FLIPPED_DIAGONALLY_FLAG   = 0x20000000;
    /*!
     * Object.hpp - ObjectFlipFlags
     */
    enum class TileFlipFlags : uint32_t
    {
            None = 0,
            Diagonally = FLIPPED_DIAGONALLY_FLAG,
            Vertically = FLIPPED_VERTICALLY_FLAG,
            Horizontally = FLIPPED_HORIZONTALLY_FLAG
    };

    /*!
     * Tileset.hpp - ObjectAlignment
     */
    enum class ObjectAlignment : uint8_t
    {
            Unspecified = 0,    //unspecified
            TopLeft = 1,        //topleft
            Top = 2,            //top
            TopRight = 3,       //topright
            Left = 4,           //left
            Center = 5,         //center
            Right = 6,          //right
            BottomLeft = 7,     //bottomleft
            Bottom = 8,         //bottom
            BottomRight = 9     //bottomright
    };

    ENABLE_BITMASK_OPERATORS(TileFlipFlags)
}

#endif //TILESON_ENUMS_HPP

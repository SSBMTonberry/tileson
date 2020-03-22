//
// Created by robin on 22.03.2020.
//

#ifndef TILESON_ENUMS_HPP
#define TILESON_ENUMS_HPP
#include <cstdint>

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
            MissingData = 3
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
}

#endif //TILESON_ENUMS_HPP

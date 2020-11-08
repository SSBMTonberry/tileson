//
// Created by robin on 08.11.2020.
//

#ifndef TILESON_ENUMBITFLAGS_HPP
#define TILESON_ENUMBITFLAGS_HPP

#include <type_traits>
#include <iostream>

namespace tson
{
    #define ENABLE_BITMASK_OPERATORS(x)  \
	template<>                           \
	struct EnableBitMaskOperators<x>     \
	{                                    \
		static const bool enable = true; \
	};

    template<typename Enum>
    struct EnableBitMaskOperators
    {
        static const bool enable = false;
    };


    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    operator |(Enum lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying>(lhs) |
                static_cast<underlying>(rhs)
        );
    }

    //Permissions operator &(Permissions lhs, Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    operator &(Enum lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying>(lhs) &
                static_cast<underlying>(rhs)
        );
    }

    //Permissions operator ^(Permissions lhs, Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    operator ^(Enum lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                static_cast<underlying>(lhs) ^
                static_cast<underlying>(rhs)
        );
    }

    //Permissions operator ~(Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    operator ~(Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        return static_cast<Enum> (
                ~static_cast<underlying>(rhs)
        );
    }

    //Permissions& operator |=(Permissions &lhs, Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    &operator |=(Enum &lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying>(lhs) |
                static_cast<underlying>(rhs)
        );

        return lhs;
    }

    //Permissions& operator &=(Permissions &lhs, Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    &operator &=(Enum &lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying>(lhs) &
                static_cast<underlying>(rhs)
        );

        return lhs;
    }

    //Permissions& operator ^=(Permissions &lhs, Permissions rhs)
    template<typename Enum>
    typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
    &operator ^=(Enum &lhs, Enum rhs)
    {
        static_assert(std::is_enum<Enum>::value,
                      "template parameter is not an enum type");

        using underlying = typename std::underlying_type<Enum>::type;

        lhs = static_cast<Enum> (
                static_cast<underlying>(lhs) ^
                static_cast<underlying>(rhs)
        );

        return lhs;
    }
}

#endif //TILESON_ENUMBITFLAGS_HPP

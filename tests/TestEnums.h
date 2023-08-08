//
// Created by robin on 07.08.23.
//

#ifndef TILESON_TESTENUMS_H
#define TILESON_TESTENUMS_H

#ifdef TILESON_UNIT_TEST_USE_SINGLE_HEADER
    #include "../tileson.hpp"
#else
    #include "../include/tileson.h"
#endif

namespace tson
{
    enum class TestEnumNumberFlags : uint32_t
    {
        None = 0,
        HasCalculatorFlag = 1 << 0,
        HasBombFlag = 1 << 1,
        HasHumorFlag = 1 << 2,
        HasInvisibilityFlag = 1 << 3,
        All = HasCalculatorFlag | HasBombFlag | HasHumorFlag | HasInvisibilityFlag
    };
    
    enum class TestEnumStringFlags : uint32_t
    {
        None = 0,
        HasCarFlag = 1 << 0,
        HasJobFlag = 1 << 1,
        HasHouseFlag = 1 << 2,
        HasMoneyFlag = 1 << 3,
        All = HasCarFlag | HasJobFlag | HasHouseFlag | HasMoneyFlag
    };
}

TILESON_ENABLE_BITMASK_OPERATORS(TestEnumNumberFlags)
TILESON_ENABLE_BITMASK_OPERATORS(TestEnumStringFlags)

#endif //TILESON_TESTENUMS_H

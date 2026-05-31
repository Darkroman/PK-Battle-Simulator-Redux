#pragma once

#include <utility>

inline constexpr std::pair<unsigned int, unsigned int> GetStageRatio(size_t stage) noexcept
{
    if (stage <= 6)
    {
        return { 2, 8 - static_cast<unsigned int>(stage) };
    }
    return { static_cast<unsigned int>(stage) - 4, 2 };
}

// You can add your accuracy ratio formula here too!
inline constexpr std::pair<unsigned int, unsigned int> GetAccuracyStageRatio(size_t stage) noexcept 
{
    if (stage <= 6)
    {
        return { 6, 18 - (static_cast<unsigned int>(stage) * 2) };
    }
    return { 6 + ((static_cast<unsigned int>(stage) - 6) * 2), 6 };
}
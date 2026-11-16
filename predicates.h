#pragma once

#include <cmath>
#include <string_view>

// на згенерованих даних обидва предикати завжди хибні (усі значення >= 0)
// тут none_of перевірить кожен елемент, найгірший випадок, мє

// одне порівняння на елемент, простий варіантік
struct IsNegative
{
    static constexpr std::string_view name = "cheap (x < 0)";

    bool operator()(int x) const { return x < 0; }
};

// двічі sin + cos на елемент, дорогий отут, а ще там завжди фолз тому ще гірше
struct TrigAboveTwo
{
    static constexpr std::string_view name = "heavy (sin/cos > 2)";

    bool operator()(int x) const
    {
        double y = std::sin(x) + std::cos(x);
        y = std::sin(y) + std::cos(y);
        return y > 2.0;
    }
};
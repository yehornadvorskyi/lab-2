#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <thread>
#include <vector>

// отут буде власний паралельний none_of:
// none_of у нас перевіряє чи не задовілняє якийсь предикат якийсь елемент з вибірки
// по умові лаби ділить вибірку на k майже рівних частин, кожну частину перевіряє звичайним none_of в окремому потоці,
// результати частин потім тре об'єднати 

template <class RandomIt, class Pred>
bool parallelNoneOf(RandomIt first, RandomIt last, Pred pred, std::size_t k)
{
    const std::size_t n = static_cast<std::size_t>(std::distance(first, last));
    k = std::max<std::size_t>(1, std::min(k, n));

    std::vector<char> partResult(k, 0);
    {
        std::vector<std::jthread> threads;
        threads.reserve(k);
        std::size_t begin = 0;
        for (std::size_t i = 0; i < k; ++i)
        {
            std::size_t end = n * (i + 1) / k;  // довжини частин відрізняються не більше ніж на 1
            threads.emplace_back([=, &partResult] {
                partResult[i] = std::none_of(first + begin, first + end, pred);
                });
            begin = end;
        }
    } // після у отута деструктори чекають завершення всіх частин

    return std::all_of(partResult.begin(), partResult.end(), [](char r) { return r != 0; });
}
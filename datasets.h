#pragma once

#include <array>
#include <cstddef>
#include <filesystem>

// оце варіантики довжини згенерованих послідовностей
inline constexpr std::array<std::size_t, 5> kSizes{ 1'000, 10'000, 100'000, 1'000'000, 10'000'000 };

// тут параметри генератора, той самий сід дасть такий же файл(аля майнкрафт)
inline constexpr unsigned kSeed = 2026;
inline constexpr int kMinValue = 0;
inline constexpr int kMaxValue = 1'000'000;

// data/seq_<size>.txt
std::filesystem::path datasetPath(std::size_t size);
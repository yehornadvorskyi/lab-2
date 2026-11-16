#pragma once

#include <filesystem>
#include <vector>

// формат для послідовності:
// лінія 1: N, кількість елементів
// лінія з 2 по N+1: цілі числа, по одному на рядок 

void saveSequence(const std::filesystem::path& path, const std::vector<int>& values);
std::vector<int> loadSequence(const std::filesystem::path& path);
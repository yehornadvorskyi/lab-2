#pragma once

#include <string>
#include <vector>

// формат для послідовності:
// лінія 1: N, кількість елементів
// лінія з 2 по N+1: цілі числа, по одному на рядок

const std::vector<std::size_t> SEQUENCE_SIZES = {10'000, 100'000, 1'000'000, 10'000'000};

// тут параметри генератора, той самий сід дасть такий же файл(аля майнкрафт)
const unsigned SEED = 2026;
const int MIN_VALUE = 0;
const int MAX_VALUE = 1'000'000;

std::string sequenceFileName(std::size_t size);

void saveSequence(const std::string& fileName, const std::vector<int>& values);
std::vector<int> loadSequence(const std::string& fileName);

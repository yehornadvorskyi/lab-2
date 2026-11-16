#pragma once

#include <cstddef>
#include <vector>

// одна згенерована послідовність з файлу
struct Dataset
{
    std::size_t size;
    std::vector<int> values;
};

std::vector<Dataset> loadDatasets();

// налаштування вимірювання
void printEnvironment(const std::vector<Dataset>& datasets);

template <class Pred>
void runLibraryExperiment(const std::vector<Dataset>& datasets, Pred pred);

// тут перевірка скіки вимірювань повернули очікуваний результат
void printCorrectness();
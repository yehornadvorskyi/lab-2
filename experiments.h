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
//отут знизу експєрімєнт буде для мого none_of
template <class Pred>
void runOwnExperiment(const Dataset& dataset, Pred pred);
// тут перевірка скіки вимірювань повернули очікуваний результат
void printCorrectness();
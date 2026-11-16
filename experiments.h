#pragma once

#include "predicates.h"

#include <functional>
#include <vector>

// загальна інфа
void printInfo(unsigned hardwareThreads);

// час одного виклику
double measureMs(const std::function<bool()>& task);

// тут експерименти 1 і 2, звичайний none_of з бібліотеки без політики і з кожною політикою
void runLibraryExperiment(const std::vector<std::vector<int>>& sequences, Predicate predicate);

// мій паралельний none_of
bool parallelNoneOf(const std::vector<int>& values, Predicate predicate, std::size_t k);

//отут знизу експєрімєнт буде для мого none_of з різною кількістю потоків K
void runOwnExperiment(const std::vector<int>& values, Predicate predicate, unsigned hardwareThreads);

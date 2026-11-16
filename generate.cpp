// генерує тестові послідовності і зберігає їх у data/seq_<size>.txt

#include "datasets.h"
#include "sequence_io.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main()
{
    filesystem::create_directories(DATA_DIR);

    mt19937 engine(kSeed);
    uniform_int_distribution<int> distribution(kMinValue, kMaxValue);

    for (size_t size : kSizes)
    {
        vector<int> values(size);
        generate(values.begin(), values.end(), [&] { return distribution(engine); });
        saveSequence(datasetPath(size), values);
        cout << "Saved " << size << " values to " << datasetPath(size).string() << '\n';
    }
}
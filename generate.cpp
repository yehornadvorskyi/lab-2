// генерує тестові послідовності і зберігає їх у data/seq_<size>.txt
// запустить один раз перед експериментами і все

#include "sequences.h"

#include <filesystem>
#include <iostream>
#include <random>

using namespace std;

int main()
{
    filesystem::create_directories(DATA_DIR);

    mt19937 generator(SEED);
    uniform_int_distribution<int> distribution(MIN_VALUE, MAX_VALUE);

    for (size_t size : SEQUENCE_SIZES)
    {
        vector<int> values(size);
        for (int& value : values)
            value = distribution(generator);

        string fileName = sequenceFileName(size);
        saveSequence(fileName, values);
        cout << "Saved " << size << " values to " << fileName << '\n';
    }
}

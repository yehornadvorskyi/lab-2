// Compiler: MSVC (Microsoft Visual Studio 2022), Standard: C++20, x64, Release configuration
// перед першим запуском зібрать і запустити generate_data бо не буде працювать без файлів(єгор не забудь)

#include "experiments.h"
#include "sequences.h"

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int main()
{
    vector<vector<int>> sequences;
    for (size_t size : SEQUENCE_SIZES)
        sequences.push_back(loadSequence(sequenceFileName(size)));

    // якщо generate_data не запускали то файлів нема і вектор пустий, далі нема шо міряти
    if (sequences.back().empty())
    {
        cout << "No data: build and run generate_data first\n";
        return 1;
    }

    unsigned h = thread::hardware_concurrency();
    if (h == 0)
        h = 4;

    printInfo(h);
    runLibraryExperiment(sequences, CHEAP);
    runLibraryExperiment(sequences, HEAVY);
    runOwnExperiment(sequences.back(), CHEAP, h);
    runOwnExperiment(sequences.back(), HEAVY, h);
}

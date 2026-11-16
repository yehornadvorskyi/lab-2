// Compiler: MSVC (Microsoft Visual Studio 2022), Standard: C++20, x64, Release configuration
// перед першим запуском зібрать і запустити generate_data бо не буде працювать без файлів(єгор не забудь)

#include "experiments.h"
#include "predicates.h"

using namespace std;

int main()
{
    const vector<Dataset> datasets = loadDatasets();
    printEnvironment(datasets);

    runLibraryExperiment(datasets, IsNegative{});
    runLibraryExperiment(datasets, TrigAboveTwo{});

    printCorrectness();
}
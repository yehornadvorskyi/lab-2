#include "datasets.h"

#include <string>

using namespace std;

filesystem::path datasetPath(size_t size)
{
    return filesystem::path(DATA_DIR) / ("seq_" + to_string(size) + ".txt");
}
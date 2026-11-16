#include "sequence_io.h"

#include <fstream>
#include <stdexcept>

using namespace std;

void saveSequence(const filesystem::path& path, const vector<int>& values)
{
    ofstream out(path);
    if (!out)
        throw runtime_error("cannot write " + path.string());

    out << values.size() << '\n';
    for (int x : values)
        out << x << '\n';
}

vector<int> loadSequence(const filesystem::path& path)
{
    ifstream in(path);
    if (!in)
        throw runtime_error("cannot read " + path.string() + " (run generate_data first)");

    size_t size = 0;
    in >> size;
    vector<int> values(size);
    for (int& x : values)
        in >> x;
    return values;
}
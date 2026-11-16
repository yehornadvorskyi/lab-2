#include "sequences.h"

#include <fstream>

using namespace std;

string sequenceFileName(size_t size)
{
    return string(DATA_DIR) + "/seq_" + to_string(size) + ".txt";
}

void saveSequence(const string& fileName, const vector<int>& values)
{
    ofstream file(fileName);
    file << values.size() << '\n';
    for (int value : values)
        file << value << '\n';
}

vector<int> loadSequence(const string& fileName)
{
    ifstream file(fileName);
    size_t count = 0;
    file >> count;

    // якщо файла нема то count лишиться 0 і вернеться пустий вектр, у мейні потів перевірю
    vector<int> values(count);
    for (size_t i = 0; i < count; ++i)
        file >> values[i];
    return values;
}

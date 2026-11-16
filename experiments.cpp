// дуже багато тєкста буде

#include "experiments.h"

#include "datasets.h"
#include "predicates.h"
#include "sequence_io.h"
#include "table.h"
#include "timing.h"

#include <algorithm>
#include <execution>
#include <format>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

namespace
{
    int measurements = 0;
    int wrongResults = 0;

    // обидва предикати на наших даних завжди хибні, тому вроді none_of має повернути true
    double checkedMs(const Measurement& m)
    {
        ++measurements;
        if (!m.value)
            ++wrongResults;
        return m.ms;
    }

    unsigned hardwareThreads()
    {
        unsigned h = thread::hardware_concurrency();
        return h == 0 ? 4 : h;  // тут 0 це невідомо
    }
}

vector<Dataset> loadDatasets()
{
    vector<Dataset> datasets;
    for (size_t size : kSizes)
        datasets.push_back({ size, loadSequence(datasetPath(size)) });
    return datasets;
}

void printEnvironment(const vector<Dataset>& datasets)
{
    cout << "Experiment: none_of\n";
    cout << "Hardware threads H: " << hardwareThreads() << '\n';
    cout << "Sequences:";
    for (const Dataset& d : datasets)
        cout << ' ' << d.size;
    cout << format("\nValues: uniform integers in [{}, {}], seed {}\n", kMinValue, kMaxValue, kSeed);
    cout << format("Time: median of {} samples, milliseconds per call\n", kRepeats);
    cout << "Both predicates are never true, so none_of checks every element\n";
}

template <class Pred>
void runLibraryExperiment(const vector<Dataset>& datasets, Pred pred)
{
    cout << format("\n== 1-2. Library none_of, predicate: {} ==\n", Pred::name);
    Table table({ "size", "no policy", "seq", "par", "unseq", "par_unseq" });
    table.printHeader();

    for (const Dataset& d : datasets)
    {
        auto first = d.values.begin();
        auto last = d.values.end();
        auto withPolicy = [&](const auto& policy) {
            return formatMs(checkedMs(measure([&] { return none_of(policy, first, last, pred); })));
            };

        table.printRow({
            to_string(d.size),
            formatMs(checkedMs(measure([&] { return none_of(first, last, pred); }))),
            withPolicy(execution::seq),
            withPolicy(execution::par),
            withPolicy(execution::unseq),
            withPolicy(execution::par_unseq),
            });
    }
}

void printCorrectness()
{
    if (wrongResults == 0)
        cout << format("\nCheck: all {} measurements returned true\n", measurements);
    else
        cout << format("\nCheck FAILED: {} of {} measurements returned false\n", wrongResults, measurements);
}

// отут шаблони експрментів, дві версії які використовує програма
template void runLibraryExperiment<IsNegative>(const vector<Dataset>&, IsNegative);
template void runLibraryExperiment<TrigAboveTwo>(const vector<Dataset>&, TrigAboveTwo);
// дуже багато тєкста буде

#include "experiments.h"

#include "datasets.h"
#include "parallel_none_of.h"
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

    vector<size_t> kValues(unsigned h)
    {
        vector<size_t> ks;
        for (size_t k = 1; k <= 2 * h; ++k)
            ks.push_back(k);
        for (size_t factor : {4, 8, 16})
            ks.push_back(factor * h);
        return ks;
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
    cout << "Build: Release, optimization " << OPT_LABEL << '\n';
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

template <class Pred>
void runOwnExperiment(const Dataset& dataset, Pred pred)
{
    const unsigned h = hardwareThreads();
    cout << format("\n== 3. Own parallel none_of, size {}, predicate: {} ==\n", dataset.size, Pred::name);
    Table table({ "K", "time, ms", "speedup" });
    table.printHeader();

    const vector<size_t> ks = kValues(h);
    vector<double> times;
    for (size_t k : ks)
    {
        times.push_back(checkedMs(measure([&] {
            return parallelNoneOf(dataset.values.begin(), dataset.values.end(), pred, k);
            })));
        table.printRow({ to_string(k), formatMs(times.back()), format("{:.2f}", times.front() / times.back()) });
    }

    const size_t best = static_cast<size_t>(min_element(times.begin(), times.end()) - times.begin());
    cout << format("Best K = {}, H = {}, K / H = {:.2f}\n", ks[best], h, static_cast<double>(ks[best]) / h);

    // на скіки К в середньому зростає час з кожним доданим потоком
    const size_t from = 2 * h - 1;
    const size_t to = ks.size() - 1;
    const double perThreadMs = (times[to] - times[from]) / static_cast<double>(ks[to] - ks[from]);
    cout << format("From K = {} to K = {}: {:+.1f} us per added thread\n", ks[from], ks[to], perThreadMs * 1000.0);
}

void printCorrectness()
{
    if (wrongResults == 0)
        cout << format("\nCheck: all {} measurements returned true\n", measurements);
    else
        cout << format("\nCheck FAILED: {} of {} measurements returned false\n", wrongResults, measurements);
}

// отут шаблони експрментів, дві(вже 4) версії які використовує програма
template void runLibraryExperiment<IsNegative>(const vector<Dataset>&, IsNegative);
template void runLibraryExperiment<TrigAboveTwo>(const vector<Dataset>&, TrigAboveTwo);
template void runOwnExperiment<IsNegative>(const Dataset&, IsNegative);
template void runOwnExperiment<TrigAboveTwo>(const Dataset&, TrigAboveTwo);
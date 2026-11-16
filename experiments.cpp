// дуже багато тєкста буде

#include "experiments.h"
#include "sequences.h"

#include <algorithm>
#include <chrono>
#include <execution>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;

// скіки разів запускається кожне вимірюваня
const int RUNS = 5;

void printInfo(unsigned hardwareThreads)
{
    cout << "Experiment: std::none_of\n";
    cout << "Build: Release, optimization " << OPT_NAME << '\n';
    cout << "Hardware threads H = " << hardwareThreads << '\n';
    cout << "Sequences:";
    for (size_t size : SEQUENCE_SIZES)
        cout << ' ' << size;
    cout << "\nValues: uniform integers from " << MIN_VALUE << " to " << MAX_VALUE << ", seed " << SEED << '\n';
    cout << "Time: median of " << RUNS << " runs, milliseconds per call\n";
    cout << "Both predicates are always false here, so none_of checks every element\n";
}

double measureMs(const function<bool()>& task)
{
    // кожен запуск міряю окремо і між запусками є виклик годинника, тому компілятор не можепорахувать none_of один раз на всі повтори (MSVC так робив, коли повтори були в одному циклі)
    vector<double> times;
    for (int run = 0; run < RUNS; ++run)
    {
        auto start = chrono::steady_clock::now();
        bool result = task();
        auto finish = chrono::steady_clock::now();
        times.push_back(chrono::duration<double, milli>(finish - start).count());

        // обидва предикати на наших даних завжди хибні, тому вроді none_of має повернути true
        if (!result)
            cout << "ERROR: none_of returned false\n";
    }
    sort(times.begin(), times.end());
    return times[RUNS / 2];   // медіана, тобто те що посередині після сортування
}

// оця штука друкує число в колонці шириною 12 сиволів, digits це скіки цифр після коми
void printCell(double value, int digits)
{
    cout << setw(12) << fixed << setprecision(digits) << value;
}

void runLibraryExperiment(const vector<vector<int>>& sequences, Predicate predicate)
{
    cout << "\n1-2. Library none_of, predicate: " << predicate.name << '\n';
    cout << setw(10) << "size" << setw(12) << "no policy" << setw(12) << "seq"
         << setw(12) << "par" << setw(12) << "unseq" << setw(12) << "par_unseq" << '\n';

    auto check = predicate.check;
    for (const vector<int>& v : sequences)
    {
        cout << setw(10) << v.size();
        printCell(measureMs([&] { return none_of(v.begin(), v.end(), check); }), 4);
        printCell(measureMs([&] { return none_of(execution::seq, v.begin(), v.end(), check); }), 4);
        printCell(measureMs([&] { return none_of(execution::par, v.begin(), v.end(), check); }), 4);
        printCell(measureMs([&] { return none_of(execution::unseq, v.begin(), v.end(), check); }), 4);
        printCell(measureMs([&] { return none_of(execution::par_unseq, v.begin(), v.end(), check); }), 4);
        cout << '\n';
    }
}

// отут буде власний паралельний none_of:
// none_of у нас перевіряє чи не задовілняє предикат жоден елемент з вибірки і по умові лаби ділить вибірку на k рівних частин, кожну частину перевіряє звичайним none_of в окремому потоці,
// результати частин потім тре об'єднати(не забути)

bool parallelNoneOf(const vector<int>& values, Predicate predicate, size_t k)
{
    const int* data = values.data();
    size_t n = values.size();

    // тут у мене результат кожної частини, char а не bool бо vector<bool> пакує все в біти і якщо різні потоки пишуть в сусідні елементи то буде некорекнтно
    vector<char> partResults(k);
    vector<thread> threads;

    for (size_t i = 0; i < k; ++i)
    {
        const int* first = data + n * i / k;
        const int* last = data + n * (i + 1) / k;
        threads.emplace_back([&partResults, i, first, last, predicate] {
            partResults[i] = none_of(first, last, predicate.check);
        });
    }
    // отут чекаємо поки всі потоки закінчать
    for (thread& t : threads)
        t.join();

    // у всій вибірці нема потрібного елемента тільки якщо його нема в жодній частині
    return all_of(partResults.begin(), partResults.end(), [](char r) { return r != 0; });
}

// шукає K де час найменший і скіки це від кількості потоків процесора (K / H)
void printBestK(const vector<size_t>& ks, const vector<double>& times, size_t h)
{
    size_t best = 0;
    for (size_t i = 1; i < times.size(); ++i)
        if (times[i] < times[best])
            best = i;

    double ratio = static_cast<double>(ks[best]) / static_cast<double>(h);
    cout << "Best K = " << ks[best] << ", H = " << h << ", K / H = " << setprecision(2) << ratio << '\n';
}

// на скіки в середньому зростає час з кожним доданим потоком
void printGrowth(const vector<size_t>& ks, const vector<double>& times, size_t h)
{
    size_t from = 2 * h - 1;      
    size_t to = ks.size() - 1;    
    double addedThreads = static_cast<double>(ks[to] - ks[from]);
    double usPerThread = (times[to] - times[from]) / addedThreads * 1000.0;
    cout << "From K = " << ks[from] << " to K = " << ks[to] << ": "
         << setprecision(1) << usPerThread << " us per added thread\n";
}

void runOwnExperiment(const vector<int>& values, Predicate predicate, unsigned hardwareThreads)
{
    size_t h = hardwareThreads;
    cout << "\n3. Own parallel none_of, " << values.size() << " elements, predicate: " << predicate.name << '\n';
    cout << setw(10) << "K" << setw(12) << "time, ms" << setw(12) << "speedup" << '\n';

    vector<size_t> ks;
    for (size_t k = 1; k <= 2 * h; ++k)
        ks.push_back(k);
    ks.push_back(4 * h);
    ks.push_back(8 * h);
    ks.push_back(16 * h);

    vector<double> times;
    for (size_t k : ks)
    {
        double ms = measureMs([&] { return parallelNoneOf(values, predicate, k); });
        times.push_back(ms);
        cout << setw(10) << k;
        printCell(ms, 4);
        printCell(times[0] / ms, 2);   // прискорення, у скіки разів швидше ніж з K = 1
        cout << '\n';
    }

    printBestK(ks, times, h);
    printGrowth(ks, times, h);
}

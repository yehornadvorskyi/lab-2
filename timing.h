#pragma once

#include <algorithm>
#include <chrono>
#include <vector>

// одне вимірюваня
struct Measurement
{
    double ms;   // середній час виклику
    bool value;  // повернення
};

inline constexpr int kRepeats = 5;            // кількість замірів при одному вимірюванні
inline constexpr double kMinSampleMs = 2.0;   // мін тривалість

// отут дуже швидкий виклик повторюється кілька разів в одному замірі, щоб було достатньо довго для годинника і повертє середнє з кількості замірів

template <class F>
Measurement measure(F&& f)
{
    using Clock = std::chrono::steady_clock;
    using Ms = std::chrono::duration<double, std::milli>;

    // перший виклик: розігрів і приблизна оцінка часу виклику

    auto start = Clock::now();
    bool value = f();
    double oneCallMs = Ms(Clock::now() - start).count();
    int callsPerSample = oneCallMs >= kMinSampleMs
        ? 1
        : static_cast<int>(kMinSampleMs / std::max(oneCallMs, 1e-6)) + 1;

    std::vector<double> samples;
    samples.reserve(kRepeats);
    for (int r = 0; r < kRepeats; ++r)
    {
        start = Clock::now();
        for (int c = 0; c < callsPerSample; ++c)
            value = f() && value;
        samples.push_back(Ms(Clock::now() - start).count() / callsPerSample);
    }

    std::nth_element(samples.begin(), samples.begin() + kRepeats / 2, samples.end());
    return { samples[kRepeats / 2], value };
}
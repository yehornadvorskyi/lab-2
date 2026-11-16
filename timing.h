#pragma once

#include <algorithm>
#include <chrono>
#include <vector>
#include <type_traits>
#include <atomic>

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

    // отут прийшлось робити виклик через volatile, бо тоді компілятор не знає, що це той самий виклик 
    // до цього компілятор обчислював іноді його один раз на весь цикл повторів і статиска не така(((
    std::remove_reference_t<F>* volatile call = &f;

    // перший виклик: розігрів і приблизна оцінка часу виклику

    auto start = Clock::now();
    bool value = (*call)();
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
        {
            // прийшлось брати допомогу ші щоб додати ще якусь перевірку, бо MSVC і далі рахує однакові виклики один раз
            // отут компілятор мусить вважати, що дані могли змінитися,тому не може взяти результат попереднього виклику замість нового
            std::atomic_signal_fence(std::memory_order_seq_cst);
            value = (*call)() && value;
        }
        samples.push_back(Ms(Clock::now() - start).count() / callsPerSample);
    }


    std::nth_element(samples.begin(), samples.begin() + kRepeats / 2, samples.end());
    return { samples[kRepeats / 2], value };
}
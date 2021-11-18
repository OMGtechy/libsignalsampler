#include "signalsampler-benchmark.hpp"
#include <signalsampler/sampler.hpp>

#include <ucontext.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

using signalsampler::get_backtrace;

template <std::size_t N>
static void BM_get_backtrace(benchmark::State& state) {
    ucontext_t context;
    getcontext(&context);

    for(auto _ : state) {
        benchmark::DoNotOptimize(get_backtrace<N>(&context));
    }
}

BENCHMARK_TEMPLATE(BM_get_backtrace, 1);
BENCHMARK_TEMPLATE(BM_get_backtrace, 8);
BENCHMARK_TEMPLATE(BM_get_backtrace, 32);
BENCHMARK_TEMPLATE(BM_get_backtrace, 48);
BENCHMARK_TEMPLATE(BM_get_backtrace, 64);
BENCHMARK_TEMPLATE(BM_get_backtrace, 96);
BENCHMARK_TEMPLATE(BM_get_backtrace, 100);
BENCHMARK_TEMPLATE(BM_get_backtrace, 256);
BENCHMARK_TEMPLATE(BM_get_backtrace, 512);
BENCHMARK_TEMPLATE(BM_get_backtrace, 1024);


#pragma once

#include <cstring>

#include <ucontext.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <signalsampler/sampler.hpp>

using signalsampler::backtrace_t;

template <std::size_t N>
backtrace_t<N> signalsampler::get_backtrace([[maybe_unused]] ucontext_t* const context) {
    unw_context_t unwindContext;

    #ifdef __aarch64__
        constexpr int flags = UNW_INIT_SIGNAL_FRAME;
        std::memcpy(&unwindContext, context, sizeof(unwindContext));
    #else
        #if UNW_VERSION >= UNW_VERSION_CODE(1,3)
            constexpr int flags = 0;
        #endif
        unw_getcontext(&unwindContext);
    #endif

    unw_cursor_t unwindCursor;

    #if UNW_VERSION < UNW_VERSION_CODE(1,3)
        #ifdef __aarch64__
            #error "AArch64 requires libunwind 1.3 or greater."
        #endif
        unw_init_local(&unwindCursor, &unwindContext);
    #else
        unw_init_local2(&unwindCursor, &unwindContext, flags);
    #endif

    backtrace_t<N> result = { };

    for (std::size_t i = 0; unw_step(&unwindCursor) == 1 && i < N; ++i) {
        unw_word_t instructionPointer = 0;
        unw_get_reg(&unwindCursor, UNW_REG_IP, &instructionPointer);
        result[i] = static_cast<instruction_pointer_t>(instructionPointer);
    }

    return result;
}

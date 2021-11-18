#pragma once

#include <cstdint>

namespace signalsampler {
    using instruction_pointer_t = uint64_t;

    template <std::size_t N>
    using backtrace_t = std::array<instruction_pointer_t, N>;
}


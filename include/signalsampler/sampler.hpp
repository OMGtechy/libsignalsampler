#pragma once

#include <cstdint>

#include <signalsampler/backtrace.hpp>

struct ucontext_t;

namespace signalsampler {
    //!
    //! \brief  Takes a sample.
    //!
    //! \tparam  N  The maximum number of entries in the backtrace.
    //!
    //! \param[in]  The signal handler context.
    //!
    //! \returns  A backtrace.
    //!
    //! \note  This function is async signal safe.
    //!
    template <std::size_t N>
    backtrace_t<N> get_backtrace(ucontext_t*);
}

#include "sampler.impl.hpp"


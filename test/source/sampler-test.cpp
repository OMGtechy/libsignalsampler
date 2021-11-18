#include "signalsampler-test.hpp"
#include <signalsampler/sampler.hpp>

#include <cstdint>
#include <functional>
#include <iostream>

#include <signal.h>

using signalsampler::get_backtrace;

namespace {
    template <typename Callable>
    [[gnu::noinline]]
    auto generate_stack_frames(const std::size_t n, Callable callable) -> decltype(callable()) {
        if (n > 0) {
            return generate_stack_frames(n - 1, std::move(callable));
        }

        return callable();
    }

    namespace test1 {
        backtrace_t<3> backtrace;
        void signal_handler(const int, siginfo_t* const, void* const ucontext) {
            std::function<backtrace_t<3>()> callable = [ucontext](){
                return get_backtrace<3>(static_cast<ucontext_t*>(ucontext));
            };

            backtrace = generate_stack_frames(10, callable);
        }
    }

    namespace test2 {
        backtrace_t<32> backtrace;
        void signal_handler(const int, siginfo_t* const, void* const ucontext) {
            std::function<backtrace_t<32>()> callable = [ucontext](){
                return get_backtrace<32>(static_cast<ucontext_t*>(ucontext));
            };

            backtrace = generate_stack_frames(3, callable);
        }
    }
}

SCENARIO("signalsampler::get_backtrace") {
    WHEN("get_backtrace<N> is called from a signal handler where N < the number of stack frames.") {
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_SIGINFO | SA_RESTART;
        action.sa_sigaction = &test1::signal_handler;

        REQUIRE(sigaction(SIGUSR1, &action, nullptr) == 0);
        REQUIRE(raise(SIGUSR1) == 0);

        // From https://man7.org/linux/man-pages/man3/raise.3.html:
        //
        // "If the signal causes a handler to be called, raise() will return
        //  only after the signal handler has returned."
        //
        // It should be safe to inspect the result now.
        UNSCOPED_INFO("Test 1 backtrace:");
        for (std::size_t i = 0; i < test1::backtrace.size(); ++i) {
            UNSCOPED_INFO("#" << i << ": " << test1::backtrace[i]);
        }

        THEN("the backtrace contains all non-zero instruction pointers.") {
            REQUIRE(test1::backtrace[0] != 0);
            REQUIRE(test1::backtrace[1] != 0);
            REQUIRE(test1::backtrace[2] != 0);
        }
    }

    WHEN("get_backtrace<N> is called from a signal handler where N > the number of stack frames.") {
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_SIGINFO | SA_RESTART;
        action.sa_sigaction = &test2::signal_handler;

        REQUIRE(sigaction(SIGUSR1, &action, nullptr) == 0);
        REQUIRE(raise(SIGUSR1) == 0);

        // From https://man7.org/linux/man-pages/man3/raise.3.html:
        //
        // "If the signal causes a handler to be called, raise() will return
        //  only after the signal handler has returned."
        //
        // It should be safe to inspect the result now.
        UNSCOPED_INFO("Test 2 backtrace:");
        for (std::size_t i = 0; i < test2::backtrace.size(); ++i) {
            UNSCOPED_INFO("#" << i << ": " << test2::backtrace[i]);
        }

        // Why 3? Because test2 generates at least 3 stack frames (see test2::signal_handler for details).
        THEN("the backtrace contains at least 3 non-zero instruction pointers.") {
            REQUIRE(test2::backtrace[0] != 0);
            REQUIRE(test2::backtrace[1] != 0);
            REQUIRE(test2::backtrace[2] != 0);
        }

        // It's hard to say exactly how many stack frames the test (including Catch2) will generate,
        // but some trial-and-error showed between 13 and 16 appearing in a debug build, and 9 in a
        // release build. So it seems safe to assume that these last few should remaining untouched
        // if all is going well.
        //
        // This may need to be reviewed when changing Catch2 versions, architectures, or compilers. 
        THEN("the last 7 instruction pointers are zero.") {
            REQUIRE(test2::backtrace[test2::backtrace.size() - 1] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 2] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 3] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 4] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 5] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 6] == 0);
            REQUIRE(test2::backtrace[test2::backtrace.size() - 7] == 0);
        }
    }
}


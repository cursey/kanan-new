#pragma once

#include <functional>

// Mimic scope(exit) from D
class ScopeExit final {
public:
    using ExitFn = std::function<void()>;

    ScopeExit(ExitFn exitfn)
        : m_exit{ exitfn }
    { }

    ~ScopeExit() {
        if (m_exit) {
            m_exit();
        }
    }

private:
    ExitFn m_exit;
};

#define SCOPE_EXIT_CONCAT_(x, y) x##y
#define SCOPE_EXIT_CONCAT(x, y) SCOPE_EXIT_CONCAT_(x, y)
#define SCOPE_EXIT(x) ScopeExit SCOPE_EXIT_CONCAT(scopeExit_, __COUNTER__){x}

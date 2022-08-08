#pragma once

#include <cstdint>

#include <SafetyHook.hpp>

namespace kanan {
    class FunctionHook {
    public:
        FunctionHook() = delete;
        FunctionHook(const FunctionHook& other) = delete;
        FunctionHook(FunctionHook&& other) = delete;
        FunctionHook(uintptr_t target, uintptr_t destination);

        // Called automatically by the destructor, but you can call it explicitly
        // if you need to remove the hook.
        bool remove();

        auto getOriginal() const {
            return m_hook->trampoline();
        }

        auto isValid() const {
            return m_hook != nullptr;
        }

        FunctionHook& operator=(const FunctionHook& other) = delete;
        FunctionHook& operator=(FunctionHook&& other) = delete;

    private:
        std::unique_ptr<SafetyHook> m_hook{};
    };
}

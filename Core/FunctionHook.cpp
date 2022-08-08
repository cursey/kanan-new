#include "FunctionHook.hpp"

using namespace std;

namespace kanan {
    std::shared_ptr<SafetyHookFactory> g_hook_factory{};

    FunctionHook::FunctionHook(uintptr_t target, uintptr_t destination) {
        if (g_hook_factory == nullptr) {
            g_hook_factory = SafetyHookFactory::init();
        }

        auto builder = g_hook_factory->acquire();
        m_hook = builder.create((void*)target, (void*)destination);
    }

    bool FunctionHook::remove() {
        m_hook.reset();
        return true;
    }
}

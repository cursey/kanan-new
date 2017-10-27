#include <Windows.h>

#include "Utility.hpp"

using namespace std;

namespace kanan {
    bool isKeyDown(int key) {
        return (GetAsyncKeyState(key) & (1 << 15)) != 0;
    }

    bool wasKeyPressed(int key) {
        static bool keys[0xFF]{ false };

        if (isKeyDown(key) && !keys[key]) {
            keys[key] = true;

            return true;
        }

        if (!isKeyDown(key)) {
            keys[key] = false;
        }

        return false;
    }
}

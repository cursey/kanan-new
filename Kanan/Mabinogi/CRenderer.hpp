#pragma once
#include ".\Matrix4x4.hpp"
#include ".\Vector3.hpp"
#include ".\Vector4.hpp"
#pragma pack(push, 1)
class CRenderer {
public:
    class CCamera {
    public:
        class CCameraState {
        public:
            char pad_0[0xc];
            Vector3 target; // 0xc
            char pad_18[0x4];
            Vector3 position; // 0x1c
            char pad_28[0x8];
            Vector4 forward; // 0x30
            float drawDistance; // 0x40
            float zNear; // 0x44
            float zFar; // 0x48
            float fov; // 0x4c
            float screenWidth; // 0x50
            float screenHeight; // 0x54
            char pad_58[0x1c];
            float aspectRatio; // 0x74
            char pad_78[0xd4];
            Matrix4x4 transformMatrix; // 0x14c
        }; // Size: 0x18c

        CCameraState* state; // 0x0
    }; // Size: 0x4

    char pad_0[0x24];
    CCamera* camera; // 0x24
}; // Size: 0x28
#pragma pack(pop)

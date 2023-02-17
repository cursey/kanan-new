#pragma once
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#pragma pack(push, 1)
class CRenderer {
public:
    class CCamera {
    public:
        class CCameraState {
        public:
            char pad_0[0x10];
            Vector3 target; // 0x10
            char pad_1c[0x4];
            Vector3 position; // 0x20
            char pad_2c[0x8];
            Vector4 forward; // 0x34
            float drawDistance; // 0x44
            float zNear; // 0x48
            float zFar; // 0x4c
            float fov; // 0x50
            float screenWidth; // 0x54
            float screenHeight; // 0x58
            char pad_5c[0x1c];
            float aspectRatio; // 0x78
            char pad_7c[0xd0];
            Matrix4x4 transformMatrix; // 0x14c
        }; // Size: 0x18c

        CCameraState* state; // 0x0
    }; // Size: 0x8

    char pad_0[0x38];
    CCamera* camera; // 0x38
    char pad_40[0xc0];
}; // Size: 0x100
#pragma pack(pop)

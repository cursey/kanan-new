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
            private: char pad_0[0x10]; public:
            Vector3 target; // 0x10
            private: char pad_1c[0x4]; public:
            Vector3 position; // 0x20
            private: char pad_2c[0x8]; public:
            Vector4 forward; // 0x34
            private: char pad_44[0x8]; public:
            float drawDistance; // 0x4c
            float fov; // 0x50
            float screenWidth; // 0x54
            float screenHeight; // 0x58
            private: char pad_5c[0x1c]; public:
            float aspectRatio; // 0x78
            private: char pad_7c[0xd0]; public:
            Matrix4x4 transformMatrix; // 0x14c
        }; // Size: 0x18c

        CCameraState* state; // 0x0
    }; // Size: 0x8

    private: char pad_0[0x38]; public:
    CCamera* camera; // 0x38
    private: char pad_40[0xc0]; public:
}; // Size: 0x100
#pragma pack(pop)

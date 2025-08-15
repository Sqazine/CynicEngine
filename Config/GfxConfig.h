#pragma once
#include <cstdint>
#include "Common.h"
namespace CynicEngine
{
    enum class GfxBackend
    {
        VULKAN,
        D3D12,
        METAL,
    };

    enum class GfxBackBufferCount : uint8_t
    {
        ONE = 1,
        TWO = 2,
        THREE = 3,

        MAX = THREE,
    };

    enum class Msaa : uint8_t
    {
        X1 = 1,
        X2,
        X4,
        X8,
        X16,
        X32,
        X64
    };

    struct GfxConfig
    {
        GfxBackend backend{GfxBackend::VULKAN};
        bool vSync{false};
        GfxBackBufferCount backBufferCount{GfxBackBufferCount::TWO};
        Msaa msaa{Msaa::X1};
#ifndef NDEBUG
        bool enableGpuValidation{true};
#endif
    };
}
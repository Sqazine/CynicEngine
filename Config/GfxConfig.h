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

    enum class GfxBackBufferCount:uint8_t
    {
        ONE = 1,
        TWO = 2,
        THREE = 3,

        MAX = THREE,
    };

    struct GfxConfig
    {
        GfxBackend backend{GfxBackend::VULKAN};
        bool vSync{false};
        GfxBackBufferCount backBufferCount{GfxBackBufferCount::TWO};
#ifndef NDEBUG
        bool enableGpuValidation{true};
#endif
    };
}
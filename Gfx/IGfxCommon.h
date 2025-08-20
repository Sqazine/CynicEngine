#pragma once

namespace CynicEngine
{
    enum class Format
    {
        R8G8B8A8_SRGB,
        B8G8R8A8_SRGB,
        B8G8R8A8_UNORM,

        R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT,

        D32_SFLOAT,
        D32_SFLOAT_S8_UINT,
        D24_UNORM_S8_UINT
    };

    enum class Filter
    {
        NEAREST,
        LINEAR
    };

    enum class AddressMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
    };
}
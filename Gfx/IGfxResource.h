#pragma once

namespace CynicEngine
{
    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;
    };

    class IGfxTexture
    {
    public:
        IGfxTexture() = default;
        virtual ~IGfxTexture() = default;
    };
}
#pragma once
#include "IGfxDevice.h"
namespace CynicEngine
{
    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;

        IGfxBuffer *Create(IGfxDevice *device);
    };

    class IGfxTexture
    {
    public:
        IGfxTexture() = default;
        virtual ~IGfxTexture() = default;

        IGfxTexture *Create(IGfxDevice *device);
    };
}
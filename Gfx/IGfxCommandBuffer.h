#pragma once
#include "IGfxDevice.h"
#include "IGfxBuffer.h"
namespace CynicEngine
{
    enum class GfxCommandType
    {
        GRAPHICS,
        COMPUTE,
        TRANSFER
    };

    class IGfxCommandBuffer
    {
    public:
        IGfxCommandBuffer() = default;
        virtual ~IGfxCommandBuffer() = default;

        static IGfxCommandBuffer *Create(IGfxDevice *device, GfxCommandType type);

        virtual IGfxCommandBuffer *Begin() = 0;
        virtual IGfxCommandBuffer *End() = 0;
        virtual IGfxCommandBuffer *CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize) = 0;
    };
}
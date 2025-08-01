#pragma once

namespace CynicEngine
{
    enum class GfxCommandType
    {
        GRAPHICS,
        COMPUTE,
        TRANSFER
    }

    class IGfxCommandBuffer
    {
    public:
        IGfxCommandBuffer() = default;
        virtual ~IGfxCommandBuffer() = default;

        static IGfxCommandBuffer *Create(IGfxDevice *device,GfxCommandType type);

        virtual void Begin() = 0;
        virtual void End() = 0;
    };
}
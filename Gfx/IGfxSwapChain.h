#pragma once

namespace CynicEngine
{
    class IGfxCommandBuffer;
    class IGfxSwapChain
    {
    public:
        IGfxSwapChain() = default;
        virtual ~IGfxSwapChain() = default;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual IGfxCommandBuffer *GetCurrentBackCommandBuffer() const = 0;

    private:
    };
}
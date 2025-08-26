#pragma once
#include "Platform/Window.h"

namespace CynicEngine
{
    class IGfxDevice;
    class IGfxCommandBuffer;
    class IGfxSwapChain
    {
    public:
        IGfxSwapChain(Window * window);
        virtual ~IGfxSwapChain() = default;

        static IGfxSwapChain *Create(IGfxDevice* device, Window *window);

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual IGfxCommandBuffer *GetCurrentBackCommandBuffer() const = 0;

        virtual uint8_t GetBackBufferCount() const = 0; 

        Window *GetWindow() const { return mWindow; }
        float GetAspect() const { return mWindow->GetAspect(); }

    protected:
        Window *mWindow;
    };
}
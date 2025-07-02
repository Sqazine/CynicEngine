#pragma once
#include "IGfxDevice.h"
#include "Platform/Window.h"
namespace CynicEngine
{
    class IGfxSwapChain
    {
    public:
        IGfxSwapChain(IGfxDevice *device, Window *window);
        virtual ~IGfxSwapChain() = default;

        static IGfxSwapChain *Create(IGfxDevice *device, Window *window);

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
    protected:
        Window *mWindow{nullptr};
    };
}
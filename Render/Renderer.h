#pragma once
#include <memory>
#include "Config/AppConfig.h"
#include "Platform/Window.h"
#include "Gfx/IGfxDevice.h"
#include "Gfx/IGfxSwapChain.h"
namespace CynicEngine
{
    class Renderer
    {
    public:
        Renderer(Window *window);
        virtual ~Renderer() = default;

        void BeginRender();
        virtual void Render();
        void EndRender();

    private:
        Window *mWindow;
        std::unique_ptr<IGfxDevice> mGfxDevice;
        std::unique_ptr<IGfxSwapChain> mGfxSwapChain;
    };
}
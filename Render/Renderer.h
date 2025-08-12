#pragma once
#include <memory>
#include <vector>
#include "Config/AppConfig.h"
#include "Platform/Window.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    class Renderer
    {
    public:
        Renderer(Window *window);
        virtual ~Renderer() = default;

        void BeginFrame();
        virtual void Render();
        void EndFrame();

    private:
        Window *mWindow;
        std::unique_ptr<IGfxDevice> mGfxDevice;
    };
}
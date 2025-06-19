#pragma once
#include <memory>
#include "Config/AppConfig.h"
#include "Platform/Window.h"
namespace CynicEngine
{
    class Renderer
    {
    public:
        Renderer(const Window* window);
        virtual ~Renderer() = default;

    private:
        const Window* mWindow;
        std::unique_ptr<IGfxDevice> mGfxDevice;
    };
}
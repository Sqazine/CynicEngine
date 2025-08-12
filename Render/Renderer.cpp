#include "Renderer.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    Renderer::Renderer(Window *window)
        : mWindow(window)
    {
        mGfxDevice.reset(IGfxDevice::Create(mWindow));
    }

    void Renderer::BeginFrame()
    {
        mGfxDevice->BeginFrame();
    }

    void Renderer::Render()
    {
    }

    void Renderer::EndFrame()
    {
        mGfxDevice->EndFrame();
    }

} // namespace CynicEngine

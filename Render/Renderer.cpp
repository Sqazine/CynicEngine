#include "Renderer.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    Renderer::Renderer(Window *window)
        : mWindow(window)
    {
        mGfxDevice.reset(IGfxDevice::Create(mWindow));
        mGfxSwapChain.reset(IGfxSwapChain::Create(mGfxDevice.get(), mWindow));
    }

    void Renderer::BeginRender()
    {
        mGfxSwapChain->BeginFrame();
    }

    void Renderer::Render()
    {
    }

    void Renderer::EndRender()
    {
        mGfxSwapChain->EndFrame();
    }

} // namespace CynicEngine

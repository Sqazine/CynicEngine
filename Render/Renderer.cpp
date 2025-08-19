#include "Renderer.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    std::unique_ptr<IGfxDevice> Renderer::mGfxDevice{nullptr};

    Renderer::Renderer(Window *window)
        : mWindow(window)
    {
        mGfxDevice.reset(IGfxDevice::Create(mWindow));
    }

    Renderer::~Renderer()
    {
        mFrameGraph.ExportGraphviz("frameGraph.gv");
        mFrameGraph.Clear();
    }

    IGfxDevice *Renderer::GetGfxDevice()
    {
        return mGfxDevice.get();
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

#include "Renderer.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    Renderer::Renderer(const Window *window)
    :mWindow(window)
    {
        const RenderConfig &renderConfig = AppConfig::GetInstance().GetRenderConfig();

        GfxDeviceDesc gfxDeviceDesc;
        gfxDeviceDesc.backend = renderConfig.backend;

        mGfxDevice.reset(IGfxDevice::Create(gfxDeviceDesc,mWindow));
    }
} // namespace CynicEngine

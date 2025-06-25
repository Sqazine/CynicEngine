#include "Renderer.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
    Renderer::Renderer(const Window *window)
    :mWindow(window)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();

        GfxDeviceDesc gfxDeviceDesc;
        gfxDeviceDesc.backend = gfxConfig.backend;

        mGfxDevice.reset(IGfxDevice::Create(gfxDeviceDesc,mWindow));
    }
} // namespace CynicEngine

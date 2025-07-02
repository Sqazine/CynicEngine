#include "IGfxSwapChain.h"
#include "Config/AppConfig.h"
#include "Core/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanSwapChain.h"
namespace CynicEngine
{
    IGfxSwapChain::IGfxSwapChain(IGfxDevice *device, Window *window)
        : mWindow(window)
    {
    }

    IGfxSwapChain *IGfxSwapChain::Create(IGfxDevice *device, Window *window)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
		switch (gfxConfig.backend)
		{
		case GfxBackend::VULKAN:
			return new GfxVulkanSwapChain(device, window);
		case GfxBackend::D3D12:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
			break;
		default:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
			break;
		}

		CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
		return nullptr;
    }
} // namespace CynicEngine
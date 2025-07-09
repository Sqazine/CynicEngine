#include "IGfxDevice.h"
#include <utility>
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanDevice.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
	IGfxDevice::IGfxDevice(const Window *window)
		: mWindow(window)
	{
	}

	IGfxDevice *IGfxDevice::Create(const Window *window)
	{
		const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
		switch (gfxConfig.backend)
		{
		case GfxBackend::VULKAN:
			// Create Vulkan device
			return new GfxVulkanDevice(window);
		case GfxBackend::D3D12:
			// Create D3D12 device
			CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
			break;
		default:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
			break;
		}

		CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
		return nullptr;
	}
}
#include "IGfxDevice.h"
#include <utility>
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanDevice.h"
#include "Config/AppConfig.h"

namespace CynicEngine
{
	IGfxDevice::IGfxDevice()
	{
	}

	IGfxDevice *IGfxDevice::Create(Window *window)
	{
		const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
		switch (gfxConfig.backend)
		{
		case GfxBackend::VULKAN:
		{
			// Create Vulkan device
			auto device = new GfxVulkanDevice();
			device->CreateSwapChain(window);
			return device;
		}
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
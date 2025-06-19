#include "IGfxDevice.h"
#include <utility>
#include "Core/Logger.h"
#include "Gfx/VK/GfxVulkanDevice.h"

namespace CynicEngine
{
	IGfxDevice::IGfxDevice(const GfxDeviceDesc &desc, const Window *window)
		: mDesc(desc), mWindow(window)
	{
	}

	IGfxDevice *IGfxDevice::Create(const GfxDeviceDesc &desc, const Window *window)
	{
		switch (desc.backend)
		{
		case GfxBackend::VULKAN:
			// Create Vulkan device
			return new GfxVulkanDevice(desc, window);
		case GfxBackend::D3D12:
			// Create D3D12 device
			CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
			break;
		default:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(desc.backend));
			break;
		}

		CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(desc.backend));
		return nullptr;
	}

	const GfxDeviceDesc &IGfxDevice::GetDesc() const
	{
		return mDesc;
	}
}
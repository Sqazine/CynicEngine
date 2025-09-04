#include "IGfxCommandBuffer.h"
#include "GfxVulkanCommandBuffer.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
namespace CynicEngine
{
	IGfxCommandBuffer *IGfxCommandBuffer::Create(IGfxDevice *device, GfxCommandType type, bool isSingleUse)
	{
		const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
		switch (gfxConfig.backend)
		{
		case GfxBackend::VULKAN:
			return new GfxVulkanCommandBuffer(device, type, isSingleUse);
		case GfxBackend::D3D12:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
			break;
		default:
			CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
			break;
		}

		CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
		return nullptr;
	}
}
#include "IGfxResource.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "VK/GfxVulkanResource.h"
namespace CynicEngine
{
    IGfxBuffer *IGfxBuffer::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanBuffer(device);
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

    IGfxTexture *IGfxTexture::Create(IGfxDevice *device)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanTexture(device);
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
}
#include "IGfxDevice.h"
#include <utility>
#include "Core/Logger.h"
#include "Gfx/VK/GfxVulkanDevice.h"
IGfxDevice::IGfxDevice(const GfxDeviceDesc &desc)
    : mDesc(desc)
{
}

IGfxDevice *IGfxDevice::Create(const GfxDeviceDesc &desc)
{
    switch (desc.backend)
    {
    case GfxBackend::VULKAN:
        // Create Vulkan device
        return new GfxVulkanDevice(desc);
    case GfxBackend::D3D12:
        // Create D3D12 device
        CYNIC_ENGINE_LOG_ERROR("Not implemented D3D12 device creation yet");
        break;
    default:
        CYNIC_ENGINE_LOG_ERROR("Unreachable GfxBackend: %d", static_cast<int>(desc.backend));
        break;
    }

    CYNIC_ENGINE_LOG_ERROR("Unreachable GfxBackend: %d", static_cast<int>(desc.backend));
    return nullptr;
}

const GfxDeviceDesc &IGfxDevice::GetDesc() const
{
    return mDesc;
}
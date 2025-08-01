#include "GfxVulkanCommon.h"

namespace CynicEngine
{
    uint32_t GetVulkanQueueFamilyIndex(const IGfxDevice *device, GfxCommandType type)
    {
        switch (type)
        {
        case GfxCommandType::GRAPHICS:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.graphicsFamilyIdx.value();
        case GfxCommandType::COMPUTE:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.computeFamilyIdx.value();
        case GfxCommandType::TRANSFER:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.transferFamilyIdx.value();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unknown GfxCommandType: {}"), static_cast<int>(type));
            return 0;
        }
    }
}
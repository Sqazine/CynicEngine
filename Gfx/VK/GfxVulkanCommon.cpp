#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxCommandBuffer.h"
#include "Gfx/IGfxResource.h"
namespace CynicEngine
{
    uint32_t GetVulkanQueueFamilyIndex(const GfxVulkanDevice *device, GfxCommandType type)
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

    VkImageAspectFlagBits GetAspectFromFormat(Format format)
    {
        switch (format)
        {
        case Format::D32_SFLOAT:
        case Format::D32_SFLOAT_S8_UINT:
        case Format::D24_UNORM_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

    VkFormat ToVkFormat(Format format)
    {
#define DEF(format)        \
    case Format::##format: \
        return VK_FORMAT_##format

        switch (format)
        {
            DEF(D24_UNORM_S8_UINT);
            DEF(D32_SFLOAT);
            DEF(D32_SFLOAT_S8_UINT);
            DEF(B8G8R8A8_SRGB);
            DEF(B8G8R8A8_UNORM);
            DEF(R32G32B32_SFLOAT);
            DEF(R8G8B8A8_SRGB);
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Format"));
        }
#undef DEF
    }

    Format ToFormat(VkFormat vkFormat)
    {
#define DEF(format)          \
    case VK_FORMAT_##format: \
        return Format::##format

        switch (vkFormat)
        {
            DEF(D24_UNORM_S8_UINT);
            DEF(D32_SFLOAT);
            DEF(D32_SFLOAT_S8_UINT);
            DEF(B8G8R8A8_SRGB);
            DEF(B8G8R8A8_UNORM);
            DEF(R32G32B32_SFLOAT);
            DEF(R8G8B8A8_SRGB);
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Format"));
        }
#undef DEF
    }

    VkFilter ToVkFilter(Filter filter)
    {
        switch (filter)
        {
        case Filter::NEAREST:
            return VK_FILTER_NEAREST;
        case Filter::LINEAR:
            return VK_FILTER_LINEAR;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan Filter"));
        }
    }

    VkSamplerAddressMode ToVkSamplerAddressMode(AddressMode addressMode)
    {
        switch (addressMode)
        {
        case AddressMode::REPEAT:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case AddressMode::MIRRORED_REPEAT:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case AddressMode::CLAMP_TO_EDGE:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan Address Mode"));
        }
    }

    VkIndexType ToVkIndexType(IndexType type)
    {
        switch (type)
        {
        case IndexType::UINT16:
            return VK_INDEX_TYPE_UINT16;
        case IndexType::UINT32:
            return VK_INDEX_TYPE_UINT32;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan index type"));
            return VK_INDEX_TYPE_UINT32;
        }
    }
}
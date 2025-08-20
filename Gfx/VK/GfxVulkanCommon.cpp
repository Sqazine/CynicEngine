#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxCommandBuffer.h"
#include "Gfx/IGfxTexture.h"
#include "Gfx/IGfxBuffer.h"
#include "Gfx/IGfxVertexDesc.h"
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
            return VK_FORMAT_R8_UNORM; // for avoiding compiler warning
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
            return Format::B8G8R8A8_UNORM; // for avoiding compiler warning
        }
#undef DEF
    }

    VkFilter ToVkFilter(Filter filter)
    {
#define DEF(filter)        \
    case Filter::##filter: \
        return VK_FILTER_##filter

        switch (filter)
        {
            DEF(NEAREST);
            DEF(LINEAR);
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan Filter"));
            return VK_FILTER_NEAREST; // for avoiding compiler warning
        }
#undef DEF
    }

    VkSamplerAddressMode ToVkSamplerAddressMode(AddressMode addressMode)
    {
#define DEF(addressMode)             \
    case AddressMode::##addressMode: \
        return VK_SAMPLER_ADDRESS_MODE_##addressMode

        switch (addressMode)
        {
            DEF(REPEAT);
            DEF(MIRRORED_REPEAT);
            DEF(CLAMP_TO_EDGE);
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan Address Mode"));
            return VK_SAMPLER_ADDRESS_MODE_REPEAT; // for avoiding compiler warning
        }
#undef DEF
    }

    VkIndexType ToVkIndexType(IGfxIndexType type)
    {
        switch (type)
        {
        case IGfxIndexType::UINT16:
            return VK_INDEX_TYPE_UINT16;
        case IGfxIndexType::UINT32:
            return VK_INDEX_TYPE_UINT32;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Vulkan index type"));
            return VK_INDEX_TYPE_UINT32; // for avoiding compiler warning
        }
    }

    VkVertexInputBindingDescription GetVulkanVertexInputBindingDescription(const IGfxVertexDesc &desc)
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = desc.bindingPoint;
        bindingDescription.stride = desc.size;
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    std::vector<VkVertexInputAttributeDescription> GetVulkanVertexInputAttributeDescriptions(const IGfxVertexDesc &desc)
    {
        std::vector<VkVertexInputAttributeDescription> result;
        for (size_t i = 0; i < desc.attribs.size(); ++i)
        {
            VkVertexInputAttributeDescription attributeDescriptions;
            attributeDescriptions.binding = desc.bindingPoint;
            attributeDescriptions.location = i;
            attributeDescriptions.format = ToVkFormat(desc.attribs[i].format);
            attributeDescriptions.offset = desc.attribs[i].offset;

            result.emplace_back(attributeDescriptions);
        }

        return result;
    }
}
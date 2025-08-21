#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxCommandBuffer.h"
#include "Gfx/IGfxTexture.h"
#include "Gfx/IGfxBuffer.h"
#include "Gfx/IGfxVertexDesc.h"
namespace CynicEngine
{
    uint32_t GetVulkanQueueFamilyIndex(const GfxVulkanDevice *device, IGfxCommandType type)
    {
        switch (type)
        {
        case IGfxCommandType::GRAPHICS:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.graphicsFamilyIdx.value();
        case IGfxCommandType::COMPUTE:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.computeFamilyIdx.value();
        case IGfxCommandType::TRANSFER:
            return device->GetPhysicalDeviceSpec().queueFamilyIndices.transferFamilyIdx.value();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unknown GfxCommandType: {}"), static_cast<int>(type));
            return 0;
        }
    }

    VkImageAspectFlagBits GetAspectFromFormat(IGfxFormat format)
    {
        switch (format)
        {
        case IGfxFormat::D32_SFLOAT:
        case IGfxFormat::D32_SFLOAT_S8_UINT:
        case IGfxFormat::D24_UNORM_S8_UINT:
            return VK_IMAGE_ASPECT_DEPTH_BIT;
        default:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

#define FormatDefines()       \
    DEF(R8G8B8A8_SRGB);       \
    DEF(B8G8R8A8_SRGB);       \
    DEF(B8G8R8A8_UNORM);      \
    DEF(R32G32_SFLOAT);       \
    DEF(R32G32B32_SFLOAT);    \
    DEF(R32G32B32A32_SFLOAT); \
    DEF(D24_UNORM_S8_UINT);   \
    DEF(D32_SFLOAT);          \
    DEF(D32_SFLOAT_S8_UINT);

    VkFormat ToVkFormat(IGfxFormat format)
    {
#define DEF(format)            \
    case IGfxFormat::##format: \
        return VK_FORMAT_##format

        switch (format)
        {
            FormatDefines();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Format"));
            return VK_FORMAT_R8_UNORM; // for avoiding compiler warning
        }
#undef DEF
    }

    IGfxFormat ToFormat(VkFormat vkFormat)
    {
#define DEF(format)          \
    case VK_FORMAT_##format: \
        return IGfxFormat::##format

        switch (vkFormat)
        {
            FormatDefines();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported Format"));
            return IGfxFormat::B8G8R8A8_UNORM; // for avoiding compiler warning
        }
#undef DEF
    }

    VkFilter ToVkFilter(IGfxFilter filter)
    {
#define DEF(filter)            \
    case IGfxFilter::##filter: \
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

    VkSamplerAddressMode ToVkSamplerAddressMode(IGfxAddressMode addressMode)
    {
#define DEF(addressMode)                 \
    case IGfxAddressMode::##addressMode: \
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
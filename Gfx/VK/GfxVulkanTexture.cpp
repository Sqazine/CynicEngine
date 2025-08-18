#include "GfxVulkanTexture.h"
#include "GfxVulkanDevice.h"
namespace CynicEngine
{
    GfxVulkanTexture::GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImage swapchainImageRawHandle)
        : IGfxTexture(desc), GfxVulkanObject(device)
    {
        if (swapchainImageRawHandle == VK_NULL_HANDLE)
        {
            mIsSwapChainImage = false;
            CreateImage(VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
        }
        else
        {
            mIsSwapChainImage = true;
            mHandle = swapchainImageRawHandle;
        }
        CreateImageView();
        CreateSampler();
    }

    GfxVulkanTexture::GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImageUsageFlags usage)
        : IGfxTexture(desc), GfxVulkanObject(device), mIsSwapChainImage(false)
    {
        CreateImage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        CreateImageView();
        CreateSampler();
    }

    GfxVulkanTexture::~GfxVulkanTexture()
    {
        mDevice->WaitIdle();
        VkDevice device = mDevice->GetLogicDevice();
        vkDestroyImageView(device, mView, nullptr);
        vkDestroySampler(device, mSampler, nullptr);
        if (!mIsSwapChainImage)
        {
            vkDestroyImage(device, mHandle, nullptr);
            vkFreeMemory(device, mMemory, nullptr);
        }
    }

    VkImageAspectFlagBits GfxVulkanTexture::GetAspect()
    {
        return GetAspectFromFormat(mDesc.format);
    }

    void GfxVulkanTexture::CreateImage(VkImageUsageFlagBits usage)
    {
        VkImageCreateInfo imageInfo{};
        ZeroVulkanStruct(imageInfo, VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO);

        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = mDesc.width;
        imageInfo.extent.height = mDesc.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mDesc.mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = ToVkFormat(mDesc.format);
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = (VkSampleCountFlagBits)mDesc.sampleCount;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK(vkCreateImage(mDevice->GetLogicDevice(), &imageInfo, nullptr, &mHandle))

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(mDevice->GetLogicDevice(), mHandle, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = mDevice->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VK_CHECK(vkAllocateMemory(mDevice->GetLogicDevice(), &allocInfo, nullptr, &mMemory))

        vkBindImageMemory(mDevice->GetLogicDevice(), mHandle, mMemory, 0);
    }

    void GfxVulkanTexture::CreateImageView()
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = mHandle;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = ToVkFormat(mDesc.format);
        viewInfo.subresourceRange.aspectMask = GetAspectFromFormat(mDesc.format);
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mDesc.mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VK_CHECK(vkCreateImageView(mDevice->GetLogicDevice(), &viewInfo, nullptr, &mView))
    }

    void GfxVulkanTexture::CreateSampler()
    {
        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = ToVkFilter(mDesc.magFilter);
        samplerInfo.minFilter = ToVkFilter(mDesc.minFilter);
        samplerInfo.addressModeU = ToVkSamplerAddressMode(mDesc.addressModeU);
        samplerInfo.addressModeV = ToVkSamplerAddressMode(mDesc.addressModeV);
        samplerInfo.addressModeW = ToVkSamplerAddressMode(mDesc.addressModeW);
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = mDevice->GetPhysicalDeviceSpec().deviceProperties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
        samplerInfo.mipLodBias = 0.0f;

        VK_CHECK(vkCreateSampler(mDevice->GetLogicDevice(), &samplerInfo, nullptr, &mSampler))
    }

}
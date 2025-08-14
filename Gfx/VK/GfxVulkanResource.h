#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxResource.h"
#include "GfxVulkanObject.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    class GfxVulkanBuffer : public GfxVulkanObject, public IGfxBuffer
    {
    public:
        GfxVulkanBuffer(IGfxDevice *device);
        ~GfxVulkanBuffer() override;
    };

    class GfxVulkanTexture : public GfxVulkanObject, public IGfxTexture
    {
    public:
        GfxVulkanTexture(IGfxDevice *device, const GfxTextureDesc &desc, VkImage swapchainImageRawHandle = VK_NULL_HANDLE);
        ~GfxVulkanTexture() override;

        VkImageAspectFlagBits GetAspect();
        VkImage GetHandle() const { return mHandle; }
        VkImageView GetView() const {return mView;}

    private:
        void CreateImage();
        void CreateImageView();
        void CreateSampler();

        VkImage mHandle;
        VkDeviceMemory mMemory;
        VkImageView mView;

        VkSampler mSampler;
        bool mIsSwapChainImage;
    };
}
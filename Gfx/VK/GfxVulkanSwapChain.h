#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include "Math/Vector2.h"
#include "GfxVulkanSyncObject.h"
#include "GfxVulkanObject.h"
#include "GfxVulkanCommandBuffer.h"
#include "GfxVulkanResource.h"
#include "Gfx/IGfxResource.h"
namespace CynicEngine
{
    struct SwapChainDetails
    {
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
    };

    class GfxVulkanSwapChain : public GfxVulkanObject
    {
    public:
        GfxVulkanSwapChain(IGfxDevice *device, Window *window);
        ~GfxVulkanSwapChain();

        void BeginFrame();
        void EndFrame();
        uint8_t GetBackBufferCount() const;
        uint8_t GetCurrentBackBufferIndex() const;
        GfxVulkanCommandBuffer *GetCurrentBackCommandBuffer() const;

        VkExtent2D GetExtent() const;

        const VkSurfaceFormatKHR GetSurfaceFormat() const;

        const VkSwapchainKHR &GetHandle() const{return mHandle;}

        uint32_t GetNextFrameIndex() const;

        GfxVulkanTexture* GetCurrentSwapChainBackTexture() const;

    private:
        void CreateSurface();
        void ObtainPresentQueue();
        void CreateSwapChain();
        void CreateBackTextures();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void BeginRender();
        void EndRender();

        SwapChainDetails QuerySwapChainDetails();

        VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void Resize(Vector2u32 extent);
        void Resize(uint32_t width, uint32_t height);
        VkResult AcquireNextImage(const GfxVulkanSemaphore *semaphore = nullptr, const GfxVulkanFence *fence = nullptr);
        void Present(const GfxVulkanSemaphore *waitFor = nullptr);

        void CleanUpResource();

        Window *mWindow;

        uint32_t mPresentFamilyIdx{0};
        VkQueue mPresentQueue{VK_NULL_HANDLE};
        VkSurfaceKHR mSurface;
        VkSwapchainKHR mHandle;

        VkSurfaceFormatKHR mSurfaceFormat;
        VkExtent2D mExtent;
        VkPresentModeKHR mPresentMode;

        uint32_t mSwapChainImageCount{0};
        uint32_t mFrameOverlapCount{0};

        uint32_t mCurrentFrameIndex{0};
        uint32_t mNextFrameIndex{0};

        std::vector<std::unique_ptr<GfxVulkanCommandBuffer>> mGfxCommandBuffer;
        std::vector<std::unique_ptr<GfxVulkanSemaphore>> mPresentSemaphore;

        std::vector<GfxVulkanTexture *> mSwapChainColorBackTextures;

        std::unique_ptr<GfxVulkanTexture> mColorBackTexture;
        std::unique_ptr<GfxVulkanTexture> mDepthBackTexture;
    };
}
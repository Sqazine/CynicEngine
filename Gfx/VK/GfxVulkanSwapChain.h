#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "GfxVulkanSyncObject.h"
#include "GfxVulkanObject.h"
#include "Gfx/IGfxSwapChain.h"
namespace CynicEngine
{
    struct SwapChainDetails
    {
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
    };

    class GfxVulkanSwapChain : public GfxVulkanObject, public IGfxSwapChain
    {
    public:
        GfxVulkanSwapChain(IGfxDevice *device, Window *window);
        ~GfxVulkanSwapChain();

        void BeginFrame() override;
        void EndFrame() override;

        VkExtent2D GetExtent() const;

        const VkSurfaceFormatKHR GetSurfaceFormat() const;

        const VkSwapchainKHR &GetHandle() const;

        uint32_t GetNextFrameIndex() const;

    private:
        void CreateSurface();
        void ObtainPresentQueue();
        void CreateSwapChain();
        void CreateSyncObjects();

        SwapChainDetails QuerySwapChainDetails();

        VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        void Resize(uint32_t width, uint32_t height);
        VkResult AcquireNextImage(const GfxVulkanSemaphore *semaphore = nullptr, const GfxVulkanFence *fence = nullptr);
        void Present(const GfxVulkanSemaphore *waitFor = nullptr);

        uint32_t mPresentFamilyIdx{0};
        VkQueue mPresentQueue{VK_NULL_HANDLE};
        VkSurfaceKHR mSurface;
        VkSwapchainKHR mHandle;

        VkSurfaceFormatKHR mSurfaceFormat;
        VkExtent2D mExtent;
        VkPresentModeKHR mPresentMode;

        uint32_t mSwapChainImageCount{0};

        std::vector<std::unique_ptr<GfxVulkanSemaphore>> mWaitSemaphores;
        std::vector<std::unique_ptr<GfxVulkanSemaphore>> mSignalSemaphores;
        std::vector<std::unique_ptr<GfxVulkanFence>> mWaitFences;
        uint32_t mCurrentFrameIndex{0};

        uint32_t mNextFrameIndex{0};
    };
}
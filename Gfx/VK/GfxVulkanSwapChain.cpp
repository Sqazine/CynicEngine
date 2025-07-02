#include "GfxVulkanSwapChain.h"
#include "GfxVulkanDevice.h"
#include "Platform/PlatformInfo.h"
#include "Config/AppConfig.h"
#include "Math/Math.hpp"
namespace CynicEngine
{
    GfxVulkanSwapChain::GfxVulkanSwapChain(IGfxDevice *device, Window *window)
        : GfxVulkanObject(device), IGfxSwapChain(device, window), mHandle(VK_NULL_HANDLE), mNextFrameIndex(0)
    {
        CreateSurface();
        ObtainPresentQueue();
        CreateSwapChain();
        CreateSyncObjects();
    }

    GfxVulkanSwapChain::~GfxVulkanSwapChain()
    {
        vkDestroySwapchainKHR(mDevice->GetLogicDevice(), mHandle, nullptr);
        vkDestroySurfaceKHR(mDevice->GetInstance(), mSurface, nullptr);
    }

    const VkSurfaceFormatKHR GfxVulkanSwapChain::GetSurfaceFormat() const
    {
        return mSurfaceFormat;
    }

    void GfxVulkanSwapChain::BeginFrame()
    {
        mWaitFences[mCurrentFrameIndex]->Wait(true, UINT64_MAX);

        auto result = vkAcquireNextImageKHR(mDevice->GetLogicDevice(), mHandle, UINT64_MAX, mWaitSemaphores[mCurrentFrameIndex]->GetHandle(), mWaitFences[mCurrentFrameIndex]->GetHandle(), &mNextFrameIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            CYNIC_ENGINE_LOG_INFO(TEXT("Swap chain out of date, resizing..."));
            Resize(mWindow->GetSize().x, mWindow->GetSize().y);
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            CYNIC_ENGINE_LOG_ERROR(TEXT("Failed to acquire next image!"));
            return;
        }

        mWaitFences[mCurrentFrameIndex]->Reset();
    }

    void GfxVulkanSwapChain::EndFrame()
    {
        VkPresentInfoKHR presentInfo;
        ZeroVulkanStruct(presentInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &mHandle;
        presentInfo.pImageIndices = &mNextFrameIndex;

        auto result = vkQueuePresentKHR(mPresentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            CYNIC_ENGINE_LOG_INFO(TEXT("Swap chain out of date, resizing..."));
            Resize(mWindow->GetSize().x, mWindow->GetSize().y);
        }
        else if (result != VK_SUCCESS)
        {
            CYNIC_ENGINE_LOG_ERROR(TEXT("Failed to present swap chain!"));
        }

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mSwapChainImageCount;
    }

    VkExtent2D GfxVulkanSwapChain::GetExtent() const
    {
        return mExtent;
    }

    VkResult GfxVulkanSwapChain::AcquireNextImage(const GfxVulkanSemaphore *semaphore, const GfxVulkanFence *fence)
    {
        return (vkAcquireNextImageKHR(mDevice->GetLogicDevice(), mHandle, UINT64_MAX, (semaphore ? semaphore->GetHandle() : nullptr), (fence ? fence->GetHandle() : nullptr), &mNextFrameIndex));
    }

    uint32_t GfxVulkanSwapChain::GetNextFrameIndex() const
    {
        return mNextFrameIndex;
    }

    void GfxVulkanSwapChain::Present(const GfxVulkanSemaphore *waitFor)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &mHandle;
        presentInfo.pImageIndices = &mNextFrameIndex;
        if (waitFor)
        {
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &waitFor->GetHandle();
        }

        VK_CHECK(vkQueuePresentKHR(mPresentQueue, &presentInfo));
    }

    void GfxVulkanSwapChain::CreateSwapChain()
    {
        SwapChainDetails swapChainDetail = QuerySwapChainDetails();
        mSurfaceFormat = ChooseSwapChainSurfaceFormat(swapChainDetail.surfaceFormats);
        mPresentMode = ChooseSwapChainPresentMode(swapChainDetail.presentModes);
        mExtent = ChooseSwapChainExtent(swapChainDetail.surfaceCapabilities);

        uint32_t imageCount = swapChainDetail.surfaceCapabilities.minImageCount;
        if (swapChainDetail.surfaceCapabilities.maxImageCount > 0 && imageCount > swapChainDetail.surfaceCapabilities.maxImageCount)
            imageCount = swapChainDetail.surfaceCapabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.surface = mSurface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = mSurfaceFormat.format;
        createInfo.imageColorSpace = mSurfaceFormat.colorSpace;
        createInfo.imageExtent = mExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = mDevice->GetPhysicalDeviceSpec().queueFamilyIndices;
        if (indices.graphicsFamilyIdx.value() != mPresentFamilyIdx)
        {
            uint32_t queueFamilyIndices[] = {indices.graphicsFamilyIdx.value(), mPresentFamilyIdx};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainDetail.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = mPresentMode;
        createInfo.oldSwapchain = mHandle == VK_NULL_HANDLE ? mHandle : VK_NULL_HANDLE;

        VK_CHECK(vkCreateSwapchainKHR(mDevice->GetLogicDevice(), &createInfo, nullptr, &mHandle));

        vkGetSwapchainImagesKHR(mDevice->GetLogicDevice(), mHandle, &mSwapChainImageCount, nullptr);
    }

    void GfxVulkanSwapChain::CreateSyncObjects()
    {
        mWaitSemaphores.resize(mSwapChainImageCount);
        mSignalSemaphores.resize(mSwapChainImageCount);
        mWaitFences.resize(mSwapChainImageCount);
        for (size_t i = 0; i < mSwapChainImageCount; i++)
        {
            mWaitSemaphores[i] = std::make_unique<GfxVulkanSemaphore>(mDevice);
            mSignalSemaphores[i] = std::make_unique<GfxVulkanSemaphore>(mDevice);

            mWaitFences[i] = std::make_unique<GfxVulkanFence>(mDevice, FenceStatus::SIGNALED);
        }
    }

    void GfxVulkanSwapChain::CreateSurface()
    {
        VulkanPlatformInfo *platformInfo = PlatformInfo::GetInstance().GetVulkanPlatformInfo();
        mSurface = platformInfo->CreateSurface(mWindow, mDevice->GetInstance());
    }

    const VkSwapchainKHR &GfxVulkanSwapChain::GetHandle() const
    {
        return mHandle;
    }

    VkSurfaceFormatKHR GfxVulkanSwapChain::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
    {
        for (const auto &availableFormat : availableFormats)
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        return availableFormats[0];
    }

    VkPresentModeKHR GfxVulkanSwapChain::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        for (const auto &availablePresentMode : availablePresentModes)
            if (gfxConfig.vSync && availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
                return availablePresentMode;

        for (const auto &availablePresentMode : availablePresentModes)
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return availablePresentMode;

        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    VkExtent2D GfxVulkanSwapChain::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR &capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() && capabilities.currentExtent.height != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;
        else
        {
            auto size = mWindow->GetSize();
            VkExtent2D actualExtent =
                {
                    (uint32_t)size.x,
                    (uint32_t)size.y,
                };

            actualExtent.width = Math::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = Math::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    SwapChainDetails GfxVulkanSwapChain::QuerySwapChainDetails()
    {
        SwapChainDetails result;

        auto physicalDevice = mDevice->GetPhysicalDeviceSpec().handle;

        uint32_t count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &count, nullptr);
        result.surfaceFormats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &count, result.surfaceFormats.data());

        count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &count, nullptr);
        result.presentModes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &count, result.presentModes.data());

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, mSurface, &result.surfaceCapabilities);

        return result;
    }

    void GfxVulkanSwapChain::ObtainPresentQueue()
    {
        auto physicalDeviceSpec = mDevice->GetPhysicalDeviceSpec();

        auto physicalDevice = physicalDeviceSpec.handle;

        auto graphicsFamilyIdx = physicalDeviceSpec.queueFamilyIndices.graphicsFamilyIdx;
        auto computeFamilyIdx = physicalDeviceSpec.queueFamilyIndices.computeFamilyIdx;
        auto transferFamilyIdx = physicalDeviceSpec.queueFamilyIndices.transferFamilyIdx;

        VkBool32 presentSupport = false;
        if (graphicsFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, graphicsFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = graphicsFamilyIdx.value();
        }
        else if (computeFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, computeFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = computeFamilyIdx.value();
        }
        else if (transferFamilyIdx.has_value())
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, transferFamilyIdx.value(), mSurface, &presentSupport);

            if (presentSupport)
                mPresentFamilyIdx = transferFamilyIdx.value();
        }
        else
        {
            CYNIC_ENGINE_LOG_ERROR(TEXT("No present queue found for the swap chain!"));
        }
        vkGetDeviceQueue(mDevice->GetLogicDevice(), mPresentFamilyIdx, 0, &mPresentQueue);
    }

    void GfxVulkanSwapChain::Resize(uint32_t width, uint32_t height)
    {
        
    }
}
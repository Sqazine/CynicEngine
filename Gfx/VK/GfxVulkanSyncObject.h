#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanObject.h"
namespace CynicEngine
{
    constexpr uint64_t FENCE_WAIT_TIME_OUT = UINT64_MAX;

    enum class FenceStatus
    {
        SIGNALED,
        UNSIGNALED,
    };

    class GfxVulkanFence : public GfxVulkanObject
    {
    public:
        GfxVulkanFence(IGfxDevice *device, FenceStatus status = FenceStatus::UNSIGNALED);
        ~GfxVulkanFence();

        const VkFence &GetHandle() const;

        void Wait(bool waitAll = true, uint64_t timeout = FENCE_WAIT_TIME_OUT);
        void Reset();

        FenceStatus GetStatus() const;

    private:
        VkFence mHandle;
        FenceStatus mStatus;
    };

    class GfxVulkanSemaphore : public GfxVulkanObject
    {
    public:
        GfxVulkanSemaphore(IGfxDevice *device);
        ~GfxVulkanSemaphore();

        const VkSemaphore &GetHandle() const;

    private:
        VkSemaphore mHandle;
    };
} // namespace CynicEngine
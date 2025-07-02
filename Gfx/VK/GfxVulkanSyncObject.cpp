#include "GfxVulkanSyncObject.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
namespace CynicEngine
{
    GfxVulkanFence::GfxVulkanFence(IGfxDevice *device, FenceStatus status)
        : GfxVulkanObject(device), mStatus(status)
    {
        VkFenceCreateInfo info;
        ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        info.pNext = nullptr;
        info.flags = mStatus == FenceStatus::SIGNALED ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        VK_CHECK(vkCreateFence(mDevice->GetLogicDevice(), &info, nullptr, &mHandle));
    }

    GfxVulkanFence::~GfxVulkanFence()
    {
        Wait();
        Reset();
        vkDestroyFence(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    const VkFence &GfxVulkanFence::GetHandle() const
    {
        return mHandle;
    }

    void GfxVulkanFence::Wait(bool waitAll, uint64_t timeout)
    {
        VkResult ret = vkWaitForFences(mDevice->GetLogicDevice(), 1, &mHandle, waitAll, timeout);
        switch (ret)
        {
        case VK_SUCCESS:
            mStatus = FenceStatus::SIGNALED;
            break;
        case VK_TIMEOUT:
            mStatus = FenceStatus::UNSIGNALED;
            CYNIC_ENGINE_LOG_ERROR(TEXT("Fence wait timeout!"));
            break;
        default:
            VK_CHECK(ret);
            break;
        }
    }

    void GfxVulkanFence::Reset()
    {
        VK_CHECK(vkResetFences(mDevice->GetLogicDevice(), 1, &mHandle));
        mStatus = FenceStatus::UNSIGNALED;
    }

    FenceStatus GfxVulkanFence::GetStatus() const
    {
        return mStatus;
    }

    GfxVulkanSemaphore::GfxVulkanSemaphore(IGfxDevice *device)
        : GfxVulkanObject(device), mHandle(VK_NULL_HANDLE)
    {
        VkSemaphoreCreateInfo info = {};
        ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        info.pNext = nullptr;
        info.flags = 0;

        VK_CHECK(vkCreateSemaphore(mDevice->GetLogicDevice(), &info, nullptr, &mHandle));
    }
    GfxVulkanSemaphore::~GfxVulkanSemaphore()
    {
        vkDestroySemaphore(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    const VkSemaphore &GfxVulkanSemaphore::GetHandle() const
    {
        return mHandle;
    }
}
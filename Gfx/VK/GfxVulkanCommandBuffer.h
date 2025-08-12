#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include "Gfx/IGfxCommandBuffer.h"
#include "GfxVulkanObject.h"
#include "GfxVulkanSyncObject.h"
namespace CynicEngine
{
    class GfxVulkanCommandBuffer : public GfxVulkanObject, public IGfxCommandBuffer
    {
    public:
        GfxVulkanCommandBuffer(IGfxDevice *device, GfxCommandType type);
        ~GfxVulkanCommandBuffer() override;

        IGfxCommandBuffer *Begin() override;
        IGfxCommandBuffer *End() override;

        IGfxCommandBuffer *Submit(GfxVulkanSemaphore *waitSemaphore);

        GfxVulkanFence *GetFence() const { return mFence.get(); }
        GfxVulkanSemaphore *GetSignalSemaphore() const { return mSignalSemaphore.get(); }

    private:
        VkQueue mRelatedQueue;
        VkCommandPool mPoolHandle;
        VkCommandBuffer mHandle;

        std::unique_ptr<GfxVulkanSemaphore> mSignalSemaphore;
        std::unique_ptr<GfxVulkanFence> mFence;
    };
}
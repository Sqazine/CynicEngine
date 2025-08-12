#include "GfxVulkanCommandBuffer.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    VkQueue QueryQueueByCommandType(IGfxDevice *device, GfxCommandType type)
    {
        auto vulkanDevice = dynamic_cast<GfxVulkanDevice *>(device);
        switch (type)
        {
        case GfxCommandType::GRAPHICS:
            return vulkanDevice->GetGraphicsQueue();
        case GfxCommandType::COMPUTE:
            return vulkanDevice->GetComputeQueue();
        case GfxCommandType::TRANSFER:
            return vulkanDevice->GetTransferQueue();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unknown GfxCommandType: {}"), static_cast<int>(type));
            return vulkanDevice->GetGraphicsQueue();
        }
    }

    GfxVulkanCommandBuffer::GfxVulkanCommandBuffer(IGfxDevice *device, GfxCommandType type)
        : GfxVulkanObject(device), mRelatedQueue(QueryQueueByCommandType(device, type)), mPoolHandle(VK_NULL_HANDLE), mHandle(VK_NULL_HANDLE)
    {
        VkCommandPoolCreateInfo poolInfo;
        ZeroVulkanStruct(poolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = GetVulkanQueueFamilyIndex(mDevice, type);
        VK_CHECK(vkCreateCommandPool(mDevice->GetLogicDevice(), &poolInfo, nullptr, &mPoolHandle));

        VkCommandBufferAllocateInfo allocInfo;
        ZeroVulkanStruct(allocInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO);
        allocInfo.commandPool = mPoolHandle;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(mDevice->GetLogicDevice(), &allocInfo, &mHandle))

        {
            mSignalSemaphore = std::make_unique<GfxVulkanSemaphore>(mDevice);

            mFence = std::make_unique<GfxVulkanFence>(mDevice, true);
        }
    }

    GfxVulkanCommandBuffer::~GfxVulkanCommandBuffer()
    {
        vkDestroyCommandPool(mDevice->GetLogicDevice(), mPoolHandle, nullptr);
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Begin()
    {
        VkCommandBufferBeginInfo beginInfo{};
        ZeroVulkanStruct(beginInfo, VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO);

        VK_CHECK(vkBeginCommandBuffer(mHandle, &beginInfo));
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::End()
    {
        VK_CHECK(vkEndCommandBuffer(mHandle))
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Submit(GfxVulkanSemaphore* waitSemaphore)
    {
        VkSemaphore waitRawSemaphore = waitSemaphore->GetHandle();
        VkSemaphore signalRawSemaphore = mSignalSemaphore->GetHandle();

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo;
        ZeroVulkanStruct(submitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitRawSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalRawSemaphore;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mHandle;

        VK_CHECK(vkQueueSubmit(mRelatedQueue, 1, &submitInfo, mFence->GetHandle()))

        return this;
    }
}
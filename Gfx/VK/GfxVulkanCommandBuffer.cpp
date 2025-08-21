#include "GfxVulkanCommandBuffer.h"
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "GfxVulkanShader.h"
#include "GfxVulkanPipeline.h"
namespace CynicEngine
{
    VkQueue QueryQueueByCommandType(IGfxDevice *device, IGfxCommandType type)
    {
        auto vulkanDevice = static_cast<GfxVulkanDevice *>(device);
        switch (type)
        {
        case IGfxCommandType::GRAPHICS:
            return vulkanDevice->GetGraphicsQueue();
        case IGfxCommandType::COMPUTE:
            return vulkanDevice->GetComputeQueue();
        case IGfxCommandType::TRANSFER:
            return vulkanDevice->GetTransferQueue();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unknown IGfxCommandType: {}"), static_cast<int>(type));
            return vulkanDevice->GetGraphicsQueue();
        }
    }

    GfxVulkanCommandBuffer::GfxVulkanCommandBuffer(IGfxDevice *device, IGfxCommandType type, bool isSingleUse)
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
            if (!isSingleUse)
            {
                mFence = std::make_unique<GfxVulkanFence>(mDevice, true);
            }
            else
            {
                mFence.reset(nullptr);
            }
        }
    }

    GfxVulkanCommandBuffer::~GfxVulkanCommandBuffer()
    {
        mDevice->WaitIdle();
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

    IGfxCommandBuffer *GfxVulkanCommandBuffer::Submit(GfxVulkanSemaphore *waitSemaphore)
    {
        VkSemaphore signalRawSemaphore = mSignalSemaphore->GetHandle();

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo;
        ZeroVulkanStruct(submitInfo, VK_STRUCTURE_TYPE_SUBMIT_INFO);
        if (waitSemaphore)
        {
            VkSemaphore waitRawSemaphore = waitSemaphore->GetHandle();
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &waitRawSemaphore;
        }
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalRawSemaphore;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &mHandle;

        if (mFence)
            VK_CHECK(vkQueueSubmit(mRelatedQueue, 1, &submitInfo, mFence->GetHandle()))
        else
            VK_CHECK(vkQueueSubmit(mRelatedQueue, 1, &submitInfo, VK_NULL_HANDLE))

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::TransitionImageLayout(GfxVulkanTexture *texture, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = texture->GetHandle();
        barrier.subresourceRange.aspectMask = texture->GetAspect();
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = 0;

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }
        else
        {
            CYNIC_ENGINE_LOG_ERROR(TEXT("unsupported layout transition!"))
        }

        vkCmdPipelineBarrier(
            GetHandle(),
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);

        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize)
    {
        auto srcVulkanBuffer = static_cast<GfxVulkanBuffer *>(src);
        auto dstVulkanBuffer = static_cast<GfxVulkanBuffer *>(dst);
        VkBufferCopy copyRegion{};
        copyRegion.size = static_cast<VkDeviceSize>(bufferSize);
        vkCmdCopyBuffer(mHandle, srcVulkanBuffer->GetHandle(), dstVulkanBuffer->GetHandle(), 1, &copyRegion);
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindRasterPipeline(IGfxRasterPipeline *pipeline)
    {
        auto vulkanRasterShader = static_cast<GfxVulkanRasterShader *>(pipeline->GetShader());
        vulkanRasterShader->Flush();

        auto sets = vulkanRasterShader->GetDescriptorSets();
        if (!sets.empty())
            vkCmdBindDescriptorSets(mHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRasterShader->GetPiplineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

        auto vulkanRasterPipeline = static_cast<GfxVulkanRasterPipeline *>(pipeline);
        vkCmdBindPipeline(mHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanRasterPipeline->GetHandle());
        return this;
    }
    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindVertexBuffer(const IGfxVertexBuffer *vertexBuffer)
    {
        auto vulkanVertexBuffer = static_cast<const GfxVulkanBuffer *>(vertexBuffer->GetGfxBuffer());
        VkBuffer vertexBuffers[] = {vulkanVertexBuffer->GetHandle()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(mHandle, 0, 1, vertexBuffers, offsets);
        return this;
    }
    IGfxCommandBuffer *GfxVulkanCommandBuffer::BindIndexBuffer(const IGfxIndexBuffer *indexBuffer)
    {
        auto vulkanIndexBuffer = static_cast<const GfxVulkanBuffer *>(indexBuffer->GetGfxBuffer());
        vkCmdBindIndexBuffer(mHandle, vulkanIndexBuffer->GetHandle(), 0, ToVkIndexType(indexBuffer->GetIndexType()));
        return this;
    }

    IGfxCommandBuffer *GfxVulkanCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
    {
        vkCmdDrawIndexed(mHandle, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        return this;
    }
}
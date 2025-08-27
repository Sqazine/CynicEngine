#pragma once
#include <vulkan/vulkan.h>
#include <memory>
#include "Gfx/IGfxCommandBuffer.h"
#include "GfxVulkanObject.h"
#include "GfxVulkanSyncObject.h"
#include "GfxVulkanTexture.h"
#include "GfxVulkanBuffer.h"
namespace CynicEngine
{
    class GfxVulkanCommandBuffer : public GfxVulkanObject, public IGfxCommandBuffer
    {
    public:
        GfxVulkanCommandBuffer(IGfxDevice *device, IGfxCommandType type, bool isSingleUse);
        ~GfxVulkanCommandBuffer() override;

        VkCommandBuffer GetHandle() const { return mHandle; }

        IGfxCommandBuffer *Begin() override;
        IGfxCommandBuffer *End() override;
        IGfxCommandBuffer *BeginRenderPass(IGfxSwapChain *swapChain) override;
        IGfxCommandBuffer *EndRenderPass() override;
        IGfxCommandBuffer *CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize) override;
        IGfxCommandBuffer *CopyBufferToImage(IGfxBuffer *src, IGfxTexture *dst, uint32_t width, uint32_t height) override;
        IGfxCommandBuffer *BindRasterPipeline(IGfxRasterPipeline *pipeline) override;
        IGfxCommandBuffer *BindVertexBuffer(const IGfxVertexBuffer *vertexBuffer) override;
        IGfxCommandBuffer *BindIndexBuffer(const IGfxIndexBuffer *indexBuffer) override;
        IGfxCommandBuffer *DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;

        IGfxCommandBuffer *Submit(GfxVulkanSemaphore *waitSemaphore = nullptr);

        IGfxCommandBuffer *TransitionImageLayout(GfxVulkanTexture *texture, VkImageLayout oldLayout, VkImageLayout newLayout);

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
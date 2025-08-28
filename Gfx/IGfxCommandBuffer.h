#pragma once
#include "IGfxDevice.h"
#include "IGfxBuffer.h"
#include "IGfxPipeline.h"
namespace CynicEngine
{
    enum class IGfxCommandType
    {
        GRAPHICS,
        COMPUTE,
        TRANSFER
    };

    class IGfxCommandBuffer
    {
    public:
        IGfxCommandBuffer() = default;
        virtual ~IGfxCommandBuffer() = default;

        static IGfxCommandBuffer *Create(IGfxDevice *device, IGfxCommandType type, bool isSingleUse = false);

        virtual IGfxCommandBuffer *Begin() = 0;
        virtual IGfxCommandBuffer *End() = 0;
        virtual IGfxCommandBuffer *BeginRenderPass(IGfxSwapChain *swapChain) = 0;
        virtual IGfxCommandBuffer *BeginRenderPass(uint8_t colorAttachmentCount, GfxTextureAttachment *colorAttachments, GfxTextureAttachment *depthAttachments) = 0;
        virtual IGfxCommandBuffer *EndRenderPass() = 0;
        virtual IGfxCommandBuffer *CopyBuffer(IGfxBuffer *src, IGfxBuffer *dst, size_t bufferSize) = 0;
        virtual IGfxCommandBuffer *CopyBufferToImage(IGfxBuffer *src, IGfxTexture *dst, uint32_t width, uint32_t height) = 0;
        virtual IGfxCommandBuffer *BindRasterPipeline(IGfxRasterPipeline *pipeline) = 0;
        virtual IGfxCommandBuffer *BindVertexBuffer(const IGfxVertexBuffer *vertexBuffer) = 0;
        virtual IGfxCommandBuffer *BindIndexBuffer(const IGfxIndexBuffer *indexBuffer) = 0;
        virtual IGfxCommandBuffer *DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
    };
}
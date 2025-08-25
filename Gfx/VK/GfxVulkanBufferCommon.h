#pragma once
#include <vector>
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanDevice.h"
#include "GfxVulkanBuffer.h"
namespace CynicEngine::GfxVulkanBufferCommon
{
    inline void SetCpuBufferData(GfxVulkanBuffer *buffer, size_t size, const void *data)
    {
        auto device = buffer->GetDevice()->GetLogicDevice();
        auto &bufferAddress = buffer->GetMappedAddress();
        vkMapMemory(device, buffer->GetMemory(), 0, size, 0, &bufferAddress);
        std::memcpy(buffer->GetMappedAddress(), data, size);
        vkUnmapMemory(device, buffer->GetMemory());
    }

    inline GfxVulkanBuffer *CreateVertexBuffer(IGfxDevice *device, const IGfxBufferDesc &gfxDesc)
    {
        GfxVulkanBufferDesc desc;
        desc.size = gfxDesc.bufferSize;
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        std::unique_ptr<GfxVulkanBuffer> stagingBuffer = std::make_unique<GfxVulkanBuffer>(device, desc);

        SetCpuBufferData(stagingBuffer.get(), (size_t)desc.size, gfxDesc.data);

        desc.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        desc.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, desc);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, IGfxCommandType::TRANSFER, true);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, desc.size)
            ->End();

        commandBuffer->Submit();

        stagingBuffer.reset();
        return result;
    }

    inline GfxVulkanBuffer *CreateIndexBuffer(IGfxDevice *device, const IGfxBufferDesc &gfxDesc)
    {
        GfxVulkanBufferDesc desc;
        desc.size = gfxDesc.bufferSize;
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        std::unique_ptr<GfxVulkanBuffer> stagingBuffer = std::make_unique<GfxVulkanBuffer>(device, desc);

        SetCpuBufferData(stagingBuffer.get(), (size_t)desc.size, gfxDesc.data);

        desc.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        desc.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, desc);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, IGfxCommandType::TRANSFER, true);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, desc.size)
            ->End();

        commandBuffer->Submit();

        stagingBuffer.reset();
        return result;
    }

    inline GfxVulkanBuffer *CreateUniformBuffer(IGfxDevice *device, const IGfxBufferDesc &gfxDesc)
    {
        GfxVulkanBufferDesc desc;
        desc.size = gfxDesc.bufferSize;
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT| VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, desc);

        SetCpuBufferData(result, (size_t)desc.size, gfxDesc.data);
        return result;
    }
}
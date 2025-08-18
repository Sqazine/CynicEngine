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
        auto bufferAddress = buffer->GetMappedAddress();
        vkMapMemory(device, buffer->GetMemory(), 0, size, 0, &bufferAddress);
        std::memcpy(buffer->GetMappedAddress(), data, size);
        vkUnmapMemory(device, buffer->GetMemory());
    }

    template <typename VertexType>
    inline GfxVulkanBuffer *CreateVertexBuffer(IGfxDevice *device, const std::vector<VertexType> &vertices)
    {
        GfxVulkanBufferDesc desc;
        desc.size = sizeof(VertexType) * vertices.size();
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        std::unique_ptr<GfxVulkanBuffer> stagingBuffer = std::make_unique<GfxVulkanBuffer>(device,desc);

        SetCpuBufferData(stagingBuffer.get(), (size_t)desc.size, vertices.data());

        desc.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        desc.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, desc);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, GfxCommandType::TRANSFER);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, desc.size)
            ->End()
            ->Submit();

        stagingBuffer.reset();
        return result;
    }

    inline GfxVulkanBuffer *CreateIndexBuffer(IGfxDevice *device, const std::vector<uint32_t> &indices)
    {
        GfxVulkanBufferDesc desc;
        desc.size = sizeof(uint32_t) * indices.size();
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        std::unique_ptr<GfxVulkanBuffer> stagingBuffer = std::make_unique<GfxVulkanBuffer>(device,desc);

        SetCpuBufferData(stagingBuffer.get(), (size_t)desc.size, indices.data());

        desc.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        desc.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        GfxVulkanBuffer *result = new GfxVulkanBuffer(device, desc);

        std::unique_ptr<GfxVulkanCommandBuffer> commandBuffer = std::make_unique<GfxVulkanCommandBuffer>(device, GfxCommandType::TRANSFER);
        commandBuffer->Begin()
            ->CopyBuffer(stagingBuffer.get(), result, desc.size)
            ->End();

        commandBuffer->Submit();

        stagingBuffer.reset();
        return result;
    }

    template <typename DataType>
    inline GfxVulkanBuffer *CreateUniformBuffer(IGfxDevice *device, const DataType &data)
    {
        GfxVulkanBufferDesc desc;
        desc.size = sizeof(DataType);
        desc.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        desc.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        GfxVulkanBuffer result = new GfxVulkanBuffer(device, desc);

        SetCpuBufferData(result, (size_t)desc.size, (void *)&data);
        return result;
    }
}
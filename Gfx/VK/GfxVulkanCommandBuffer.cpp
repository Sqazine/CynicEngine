#include "GfxVulkanCommandBuffer.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    GfxVulkanCommandBuffer::GfxVulkanCommandBuffer(IGfxDevice *device,GfxCommandType type)
        : GfxVulkanObject(device), mPoolHandle(VK_NULL_HANDLE), mHandle(VK_NULL_HANDLE)
    {
        VkCommandPoolCreateInfo poolInfo;
        ZeroVulkanStruct(poolInfo, VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO);
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = GetVulkanQueueFamilyIndex(mDevice, type);
    }

    GfxVulkanCommandBuffer::~GfxVulkanCommandBuffer()
    {
    }

    void GfxVulkanCommandBuffer::Begin()
    {
    }

    void GfxVulkanCommandBuffer::End()
    {
    }
}
#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxCommandBuffer.h"
#include "GfxVulkanObject.h"
namespace CynicEngine
{
    class GfxVulkanCommandBuffer : public GfxVulkanObject, public IGfxCommandBuffer
    {
    public:
        GfxVulkanCommandBuffer(IGfxDevice *device,GfxCommandType type);
        ~GfxVulkanCommandBuffer() override ;

        IGfxCommandBuffer* Begin() override;
        IGfxCommandBuffer *End() override;

    private:
        VkQueue mRelatedQueue;
        VkCommandPool mPoolHandle;
        VkCommandBuffer mHandle;
    };
}
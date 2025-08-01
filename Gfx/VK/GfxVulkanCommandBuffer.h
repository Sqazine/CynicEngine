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

        void Begin() override;
        void End() override;

    private:
        VkCommandPool mPoolHandle;
        VkCommandBuffer mHandle;
    };
}
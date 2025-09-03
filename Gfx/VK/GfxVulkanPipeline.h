#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxPipeline.h"
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanObject.h"
namespace CynicEngine
{
    class GfxVulkanRasterPipeline : public GfxVulkanObject, public IGfxRasterPipeline
    {
    public:
        GfxVulkanRasterPipeline(IGfxDevice *device, const IGfxRasterPipelineDesc & pipelineDesc);
        ~GfxVulkanRasterPipeline() override;

        VkPipeline GetHandle() const { return mHandle; }

    private:
        void Create();

        VkVertexInputBindingDescription mVertexInputBindingState{};
        std::vector<VkVertexInputAttributeDescription> mVertexAttributes{};

        VkPipeline mHandle;
    };
}
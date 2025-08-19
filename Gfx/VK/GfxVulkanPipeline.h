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
        GfxVulkanRasterPipeline(IGfxDevice *device, const IGfxVertexDesc &vertexDesc, IGfxRasterShader *shader);
        ~GfxVulkanRasterPipeline() override;

    private:
        void InitStates(const IGfxVertexDesc &vertexDesc);
        void Create();

        VkPipelineVertexInputStateCreateInfo mVertexState{};
        VkPipelineInputAssemblyStateCreateInfo mInputAssemblyState{};
        VkPipelineViewportStateCreateInfo mViewportState{};
        VkPipelineRasterizationStateCreateInfo mRasterizerState{};
        VkPipelineMultisampleStateCreateInfo mMultisamplingState{};
        VkPipelineDepthStencilStateCreateInfo mDepthStencilState{};
        VkPipelineColorBlendStateCreateInfo mColorBlendState{};
        VkPipelineDynamicStateCreateInfo mDynamicState{};

        VkPipelineRenderingCreateInfoKHR mPipelineRenderingCreateInfo{};

        VkPipeline mHandle;
    };
}
#include "GfxVulkanPipeline.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanDevice.h"
#include "GfxVulkanShader.h"
#include "Config/AppConfig.h"
#include "Config/GfxConfig.h"
namespace CynicEngine
{
    GfxVulkanRasterPipeline::GfxVulkanRasterPipeline(IGfxDevice *device, const IGfxVertexDesc &vertexDesc, IGfxRasterShader *shader)
        : GfxVulkanObject(device), IGfxRasterPipeline(shader)
    {
        Create(vertexDesc);
    }

    GfxVulkanRasterPipeline::~GfxVulkanRasterPipeline()
    {
        mDevice->WaitIdle();
        vkDestroyPipeline(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    void GfxVulkanRasterPipeline::Create(const IGfxVertexDesc &vertexDesc)
    {
        mVertexInputBindingState = GetVulkanVertexInputBindingDescription(vertexDesc);
        mVertexAttributes = GetVulkanVertexInputAttributeDescriptions(vertexDesc);

        VkPipelineVertexInputStateCreateInfo vertexState;
        ZeroVulkanStruct(vertexState, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
        vertexState.vertexBindingDescriptionCount = 1;
        vertexState.pVertexBindingDescriptions = &mVertexInputBindingState;
        vertexState.vertexAttributeDescriptionCount = static_cast<uint32_t>(mVertexAttributes.size());
        vertexState.pVertexAttributeDescriptions = mVertexAttributes.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
        ZeroVulkanStruct(inputAssemblyState, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
        inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyState.primitiveRestartEnable = VK_FALSE;

        auto extent = static_cast<GfxVulkanSwapChain *>(mDevice->GetSwapChain())->GetExtent();
        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = extent.width;
        viewport.height = extent.height;
        viewport.minDepth = 0.0;
        viewport.maxDepth = 1.0;

        VkRect2D scissor;
        scissor.offset = {0, 0};
        scissor.extent = extent;

        VkPipelineViewportStateCreateInfo viewportState;
        ZeroVulkanStruct(viewportState, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerState;
        ZeroVulkanStruct(rasterizerState, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
        rasterizerState.depthClampEnable = VK_FALSE;
        rasterizerState.rasterizerDiscardEnable = VK_FALSE;
        rasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerState.lineWidth = 1.0f;
        rasterizerState.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizerState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerState.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multiSamplingState;
        ZeroVulkanStruct(multiSamplingState, VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
        multiSamplingState.sampleShadingEnable = VK_FALSE;
        multiSamplingState.rasterizationSamples = static_cast<VkSampleCountFlagBits>(AppConfig::GetInstance().GetGfxConfig().msaa);

        VkPipelineDepthStencilStateCreateInfo depthStencilState;
        ZeroVulkanStruct(depthStencilState, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
        depthStencilState.depthTestEnable = VK_TRUE;
        depthStencilState.depthWriteEnable = VK_TRUE;
        depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencilState.depthBoundsTestEnable = VK_FALSE;
        depthStencilState.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlendState;
        ZeroVulkanStruct(colorBlendState, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
        colorBlendState.logicOpEnable = VK_FALSE;
        colorBlendState.logicOp = VK_LOGIC_OP_COPY;
        colorBlendState.attachmentCount = 1;
        colorBlendState.pAttachments = &colorBlendAttachment;
        colorBlendState.blendConstants[0] = 0.0f;
        colorBlendState.blendConstants[1] = 0.0f;
        colorBlendState.blendConstants[2] = 0.0f;
        colorBlendState.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
        };

        auto vulkanSwapChain = static_cast<GfxVulkanSwapChain *>(mDevice->GetSwapChain());

        VkPipelineDynamicStateCreateInfo dynamicState;
        ZeroVulkanStruct(dynamicState, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();
        VkFormat colorAttachmentFormats[] = {
            vulkanSwapChain->GetColorTextureFormat(),
        };

        VkPipelineRenderingCreateInfoKHR pipelineRendering;
        ZeroVulkanStruct(pipelineRendering, VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR);
        pipelineRendering.pNext = nullptr;
        pipelineRendering.colorAttachmentCount = colorBlendState.attachmentCount;
        pipelineRendering.pColorAttachmentFormats = colorAttachmentFormats;
        pipelineRendering.depthAttachmentFormat = vulkanSwapChain->GetDepthTextureFormat();
        pipelineRendering.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

        auto rawShader = static_cast<GfxVulkanRasterShader *>(mShader);

        VkGraphicsPipelineCreateInfo pipelineInfo;
        ZeroVulkanStruct(pipelineInfo, VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
        pipelineInfo.pNext = &pipelineRendering;
        pipelineInfo.stageCount = rawShader->GetPipelineShaderStageInfoList().size();
        pipelineInfo.pStages = rawShader->GetPipelineShaderStageInfoList().data();
        pipelineInfo.pVertexInputState = &vertexState;
        pipelineInfo.pInputAssemblyState = &inputAssemblyState;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizerState;
        pipelineInfo.pMultisampleState = &multiSamplingState;
        pipelineInfo.pDepthStencilState = &depthStencilState;
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.layout = rawShader->GetPiplineLayout();

        VK_CHECK(vkCreateGraphicsPipelines(mDevice->GetLogicDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mHandle))
    }
}
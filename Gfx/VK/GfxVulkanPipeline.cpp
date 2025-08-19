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
        InitStates(vertexDesc);
        Create();
    }

    GfxVulkanRasterPipeline::~GfxVulkanRasterPipeline()
    {
        mDevice->WaitIdle();
        vkDestroyPipeline(mDevice->GetLogicDevice(), mHandle, nullptr);
    }

    void GfxVulkanRasterPipeline::InitStates(const IGfxVertexDesc &vertexDesc)
    {
        ZeroVulkanStruct(mVertexState, VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO);
        ZeroVulkanStruct(mInputAssemblyState, VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO);
        ZeroVulkanStruct(mViewportState, VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO);
        ZeroVulkanStruct(mRasterizerState, VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO);
        ZeroVulkanStruct(mMultisamplingState, VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO);
        ZeroVulkanStruct(mDepthStencilState, VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO);
        ZeroVulkanStruct(mColorBlendState, VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO);
        ZeroVulkanStruct(mDynamicState, VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO);
        ZeroVulkanStruct(mPipelineRenderingCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR);

        auto bindingDescription = GetVulkanVertexInputBindingDescription(vertexDesc);
        auto attributeDescriptions = GetVulkanVertexInputAttributeDescriptions(vertexDesc);

        mVertexState.vertexBindingDescriptionCount = 1;
        mVertexState.pVertexBindingDescriptions = &bindingDescription;
        mVertexState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        mVertexState.pVertexAttributeDescriptions = attributeDescriptions.data();

        mInputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        mInputAssemblyState.primitiveRestartEnable = VK_FALSE;

        mViewportState.viewportCount = 1;
        mViewportState.scissorCount = 1;

        mRasterizerState.depthClampEnable = VK_FALSE;
        mRasterizerState.rasterizerDiscardEnable = VK_FALSE;
        mRasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
        mRasterizerState.lineWidth = 1.0f;
        mRasterizerState.cullMode = VK_CULL_MODE_BACK_BIT;
        mRasterizerState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        mRasterizerState.depthBiasEnable = VK_FALSE;

        mMultisamplingState.sampleShadingEnable = VK_FALSE;
        mMultisamplingState.rasterizationSamples = static_cast<VkSampleCountFlagBits>(AppConfig::GetInstance().GetGfxConfig().msaa);

        mDepthStencilState.depthTestEnable = VK_TRUE;
        mDepthStencilState.depthWriteEnable = VK_TRUE;
        mDepthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
        mDepthStencilState.depthBoundsTestEnable = VK_FALSE;
        mDepthStencilState.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        mColorBlendState.logicOpEnable = VK_FALSE;
        mColorBlendState.logicOp = VK_LOGIC_OP_COPY;
        mColorBlendState.attachmentCount = 1;
        mColorBlendState.pAttachments = &colorBlendAttachment;
        mColorBlendState.blendConstants[0] = 0.0f;
        mColorBlendState.blendConstants[1] = 0.0f;
        mColorBlendState.blendConstants[2] = 0.0f;
        mColorBlendState.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR};

        mDynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        mDynamicState.pDynamicStates = dynamicStates.data();

        mPipelineRenderingCreateInfo.pNext = nullptr;
        mPipelineRenderingCreateInfo.colorAttachmentCount = mColorBlendState.attachmentCount;
        // mPipelineRenderingCreateInfo.pColorAttachmentFormats = GetVulkanContextInstance()->GetSwapChainImageFormat();
        // mPipelineRenderingCreateInfo.depthAttachmentFormat = ToVkFormat(GetVulkanContextInstance()->FindDepthFormat());
        mPipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
    }

    void GfxVulkanRasterPipeline::Create()
    {
        auto rawShader = dynamic_cast<GfxVulkanRasterShader *>(mShader);

        VkGraphicsPipelineCreateInfo pipelineInfo;
        ZeroVulkanStruct(pipelineInfo,VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO);
        pipelineInfo.pNext = &mPipelineRenderingCreateInfo;
        pipelineInfo.stageCount = rawShader->GetPipelineShaderStageInfoList().size();
        pipelineInfo.pStages = rawShader->GetPipelineShaderStageInfoList().data();
        pipelineInfo.pVertexInputState = &mVertexState;
        pipelineInfo.pInputAssemblyState = &mInputAssemblyState;
        pipelineInfo.pViewportState = &mViewportState;
        pipelineInfo.pRasterizationState = &mRasterizerState;
        pipelineInfo.pMultisampleState = &mMultisamplingState;
        pipelineInfo.pDepthStencilState = &mDepthStencilState;
        pipelineInfo.pColorBlendState = &mColorBlendState;
        pipelineInfo.pDynamicState = &mDynamicState;
        pipelineInfo.layout = rawShader->GetPiplineLayout();
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateGraphicsPipelines(mDevice->GetLogicDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mHandle))
    }
}
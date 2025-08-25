#pragma once
#if _WIN32
#pragma comment(lib, "ucrtd.lib")
#endif
#include <vulkan/vulkan.h>
#include <string_view>
#include <vector>
#include <spirv_reflect.h>
#include <unordered_map>
#include "Gfx/IGfxShader.h"
#include "GfxVulkanObject.h"

namespace CynicEngine
{
    struct SpirvReflectedData
    {
        std::vector<SpvReflectInterfaceVariable *> inputVariables;
        std::vector<SpvReflectInterfaceVariable *> ouputVariables;
        std::vector<SpvReflectBlockVariable *> pushConstants;
        std::vector<SpvReflectDescriptorSet *> descriptorSets;
        std::vector<SpvReflectDescriptorBinding *> descriptorBindings;
    };

    class GfxVulkanRasterShader : public GfxVulkanObject, public IGfxRasterShader
    {
    public:
        GfxVulkanRasterShader(IGfxDevice *device,
                              std::string_view vertContent,
                              std::string_view fragContent,
                              std::string_view tessCtrlContent,
                              std::string_view tessEvalContent,
                              std::string_view geomContent);
        ~GfxVulkanRasterShader() override;

        const std::vector<VkPipelineShaderStageCreateInfo> &GetPipelineShaderStageInfoList() const;

        std::vector<VkDescriptorSetLayoutBinding> GetDescriptorLayoutBindingList();
        const std::vector<VkDescriptorSetLayout> &GetDescriptorSetLayoutList() const;
        const std::vector<VkDescriptorSet> &GetDescriptorSets();
        VkPipelineLayout GetPiplineLayout() const;

        IGfxRasterShader *BindBuffer(std::string_view name, const IGfxBuffer *buffer) override;
        IGfxRasterShader *BindTexture(std::string_view name, const IGfxTexture *texture) override;

        void Flush();

    private:
        void CreateFromFiles(std::string_view vertContent,
                             std::string_view fragContent,
                             std::string_view tessCtrlContent,
                             std::string_view tessEvalContent,
                             std::string_view geomContent);

        SpirvReflectedData SpirvReflect(size_t idx, std::string_view spvCode);

        VkShaderModule CreateShaderModule(std::string_view content);

        void DumpDescriptorBindings();
        void DumpDescriptorSetLayouts();
        void CreateDescriptorPool();
        void AllocateDescriptorSets();
        void DumpDescriptorWrites();
        void CreatePipelineLayout();

        const VkDescriptorPool &GetDescriptorPool() const;

        std::vector<VkWriteDescriptorSet> GetWrites();
        bool CheckDescriptorWriteValid();

        VkShaderStageFlagBits GetShaderStageFlag(size_t idx);

        void MarkDirty();

        bool mIsDirty{true};

        SpvReflectShaderModule mSpvModule[5];
        SpirvReflectedData mReflectedData[5];

        VkShaderModule mShaderModule[5];
        std::vector<VkPipelineShaderStageCreateInfo> mStageCreateInfos;

        std::unordered_map<std::string_view, VkDescriptorSetLayoutBinding> mBindings;
        std::unordered_map<std::string_view, VkWriteDescriptorSet> mWrites;

        std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
        std::vector<VkDescriptorSet> mDescriptorSets;
        VkDescriptorPool mDescriptorPool{VK_NULL_HANDLE};

        VkPipelineLayout mPipelineLayout{VK_NULL_HANDLE};

        std::unordered_map<std::string_view, VkDescriptorBufferInfo> mBufferInfos;
        std::unordered_map<std::string_view, VkDescriptorImageInfo> mImageInfos;
    };
}
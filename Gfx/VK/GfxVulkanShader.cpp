#include "GfxVulkanShader.h"
#include "GfxVulkanDevice.h"
#include "GfxVulkanCommon.h"
#include "GfxVulkanResource.h"
#include <cassert>

namespace CynicEngine
{

    GfxVulkanRasterShader::GfxVulkanRasterShader(IGfxDevice *device,
                                                 std::string_view vertContent,
                                                 std::string_view fragContent,
                                                 std::string_view tessCtrlContent,
                                                 std::string_view tessEvalContent,
                                                 std::string_view geomContent)
        : GfxVulkanObject(device)
    {
        for (int i = 0; i < 5; i++)
        {
            mShaderModule[i] = VK_NULL_HANDLE;
            mSpvModule[i] = {};
        }

        CreateFromFiles(vertContent, fragContent, tessCtrlContent, tessEvalContent, geomContent);
    }
    GfxVulkanRasterShader::~GfxVulkanRasterShader()
    {
        VkDevice device = mDevice->GetLogicDevice();

        vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);

        for (auto &setLayout : mDescriptorSetLayouts)
        {
            vkDestroyDescriptorSetLayout(device, setLayout, nullptr);
        }

        vkDestroyDescriptorPool(device, mDescriptorPool, nullptr);

        for (int i = 0; i < 5; i++)
        {
            if (mSpvModule[i].entry_point_name != nullptr)
                spvReflectDestroyShaderModule(&mSpvModule[i]);

            if (mShaderModule[i] != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(device, mShaderModule[i], nullptr);
                mShaderModule[i] = VK_NULL_HANDLE;
            }
        }
        for (auto &info : mStageCreateInfos)
        {
            info = {};
        }
    }

    void GfxVulkanRasterShader::CreateFromFiles(std::string_view vertContent,
                                                std::string_view fragContent,
                                                std::string_view tessCtrlContent,
                                                std::string_view tessEvalContent,
                                                std::string_view geomContent)
    {
        mStageCreateInfos.clear();

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
        ZeroVulkanStruct(shaderStageCreateInfo, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);

        mShaderModule[0] = CreateShaderModule(vertContent);
        mReflectedData[0] = SpirvReflect(0, vertContent);
        shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageCreateInfo.module = mShaderModule[0];
        shaderStageCreateInfo.pName = "main";
        mStageCreateInfos.emplace_back(shaderStageCreateInfo);

        mShaderModule[1] = CreateShaderModule(fragContent);
        mReflectedData[1] = SpirvReflect(1, fragContent);
        shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageCreateInfo.module = mShaderModule[1];
        shaderStageCreateInfo.pName = "main";
        mStageCreateInfos.emplace_back(shaderStageCreateInfo);

        if (!tessCtrlContent.empty())
        {
            mShaderModule[2] = CreateShaderModule(tessCtrlContent);
            mReflectedData[2] = SpirvReflect(2, tessCtrlContent);
            shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            shaderStageCreateInfo.module = mShaderModule[2];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        if (!tessEvalContent.empty())
        {

            mShaderModule[3] = CreateShaderModule(tessEvalContent);
            mReflectedData[3] = SpirvReflect(3, tessEvalContent);
            shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            shaderStageCreateInfo.module = mShaderModule[3];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        if (!geomContent.empty())
        {
            mShaderModule[4] = CreateShaderModule(geomContent);
            mReflectedData[4] = SpirvReflect(4, geomContent);
            shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageCreateInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            shaderStageCreateInfo.module = mShaderModule[4];
            shaderStageCreateInfo.pName = "main";
            mStageCreateInfos.emplace_back(shaderStageCreateInfo);
        }

        DumpDescriptorBindings();
        DumpDescriptorSetLayouts();
        CreateDescriptorPool();
        AllocateDescriptorSets();
        DumpDescriptorWrites();
        CreatePipelineLayout();
    }

    const std::vector<VkPipelineShaderStageCreateInfo> &GfxVulkanRasterShader::GetPipelineShaderStageInfoList() const
    {
        return mStageCreateInfos;
    }

    std::vector<VkDescriptorSetLayoutBinding> GfxVulkanRasterShader::GetDescriptorLayoutBindingList()
    {
        std::vector<VkDescriptorSetLayoutBinding> result;
        for (auto [k, v] : mBindings)
        {
            result.emplace_back(v);
        }

        return result;
    }

    const std::vector<VkDescriptorSetLayout> &GfxVulkanRasterShader::GetDescriptorSetLayoutList() const
    {
        return mDescriptorSetLayouts;
    }

    const VkDescriptorPool &GfxVulkanRasterShader::GetDescriptorPool() const
    {
        return mDescriptorPool;
    }

    const std::vector<VkDescriptorSet> &GfxVulkanRasterShader::GetDescriptorSets()
    {
        return mDescriptorSets;
    }

    VkPipelineLayout GfxVulkanRasterShader::GetPiplineLayout() const
    {
        return mPipelineLayout;
    }

    IGfxRasterShader *GfxVulkanRasterShader::BindBuffer(std::string_view name, const IGfxBuffer *buffer)
    {
        MarkDirty();

        if (mBufferInfos.find(name) == mBufferInfos.end())
            mBufferInfos[name] = VkDescriptorBufferInfo{};

        auto rawBuffer = dynamic_cast<const GfxVulkanBuffer *>(buffer);

        // mBufferInfos[name].buffer = rawBuffer->GetHandle();
        // mBufferInfos[name].offset = 0;
        // mBufferInfos[name].range = rawBuffer->GetSize();

        mWrites[name].pBufferInfo = &mBufferInfos[name];

        return this;
    }

    IGfxRasterShader *GfxVulkanRasterShader::BindTexture(std::string_view name, const IGfxTexture *texture)
    {
        MarkDirty();

        auto rawTexture = dynamic_cast<const GfxVulkanTexture *>(texture);

        if (mImageInfos.find(name) == mImageInfos.end())
            mImageInfos[name] = VkDescriptorImageInfo{};

        // mImageInfos[name].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        // mImageInfos[name].imageView = rawTexture->mImage->mView;
        // mImageInfos[name].sampler = rawTexture->mSampler;

        mWrites[name].pImageInfo = &mImageInfos[name];
        return this;
    }

    void GfxVulkanRasterShader::Flush()
    {
        if (mIsDirty)
        {
            mIsDirty = false;

            auto writeList = GetWrites();
            vkUpdateDescriptorSets(mDevice->GetLogicDevice(), static_cast<uint32_t>(writeList.size()), writeList.data(), 0, nullptr);
        }
    }

#define SPIRV_REFLECT_CHECK(v)                   \
    do                                           \
    {                                            \
        assert(v == SPV_REFLECT_RESULT_SUCCESS); \
    } while (false);

    SpirvReflectedData GfxVulkanRasterShader::SpirvReflect(size_t idx, std::string_view spvCode)
    {
        SpirvReflectedData result;

        SPIRV_REFLECT_CHECK(spvReflectCreateShaderModule(spvCode.size(), (const void *)spvCode.data(), &mSpvModule[idx]));

        uint32_t varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateInputVariables(&mSpvModule[idx], &varCount, nullptr));
        result.inputVariables.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateInputVariables(&mSpvModule[idx], &varCount, result.inputVariables.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateOutputVariables(&mSpvModule[idx], &varCount, nullptr));
        result.ouputVariables.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateOutputVariables(&mSpvModule[idx], &varCount, result.ouputVariables.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorBindings(&mSpvModule[idx], &varCount, nullptr));
        result.descriptorBindings.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorBindings(&mSpvModule[idx], &varCount, result.descriptorBindings.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorSets(&mSpvModule[idx], &varCount, nullptr));
        result.descriptorSets.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumerateDescriptorSets(&mSpvModule[idx], &varCount, result.descriptorSets.data()));

        varCount = 0;
        SPIRV_REFLECT_CHECK(spvReflectEnumeratePushConstantBlocks(&mSpvModule[idx], &varCount, nullptr));
        result.pushConstants.resize(varCount);
        SPIRV_REFLECT_CHECK(spvReflectEnumeratePushConstantBlocks(&mSpvModule[idx], &varCount, result.pushConstants.data()));

        return result;
    }

    VkShaderModule GfxVulkanRasterShader::CreateShaderModule(std::string_view content)
    {
        VkShaderModuleCreateInfo createInfo;
        ZeroVulkanStruct(createInfo, VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO);
        createInfo.codeSize = content.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(content.data());

        VkShaderModule shaderModule;
        VK_CHECK(vkCreateShaderModule(mDevice->GetLogicDevice(), &createInfo, nullptr, &shaderModule))

        return shaderModule;
    }

    void GfxVulkanRasterShader::DumpDescriptorBindings()
    {
        for (size_t i = 0; i < 5; ++i)
        {
            for (auto &spvBinding : mReflectedData[i].descriptorBindings)
            {
                for (auto &vkBinding : mBindings)
                {
                    if (vkBinding.first == spvBinding->name)
                    {
                        vkBinding.second.stageFlags = GetShaderStageFlag(i);
                    }
                }
                VkDescriptorSetLayoutBinding layoutBinding{};
                layoutBinding.binding = spvBinding->binding;
                layoutBinding.descriptorCount = spvBinding->count;
                layoutBinding.descriptorType = (VkDescriptorType)spvBinding->descriptor_type;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBinding.stageFlags = GetShaderStageFlag(i);
                mBindings[spvBinding->name] = layoutBinding;
            }
        }
    }

    void GfxVulkanRasterShader::DumpDescriptorSetLayouts()
    {
        size_t maxCount = 0;
        for (size_t i = 0; i < 5; ++i)
        {
            for (auto &spvSet : mReflectedData[i].descriptorSets)
            {
                if (maxCount < spvSet->set)
                {
                    maxCount = spvSet->set;
                }
            }
        }

        mDescriptorSetLayouts.resize(maxCount + 1);

        auto GetDescriptorBinding = [&](std::string_view name)
        {
            for (auto &[k, v] : mBindings)
            {
                if (k == name)
                    return v;
            }

            return VkDescriptorSetLayoutBinding{};
        };

        for (size_t i = 0; i < 5; ++i)
        {
            for (auto &spvSet : mReflectedData[i].descriptorSets)
            {
                auto setSlot = spvSet->set;

                std::vector<VkDescriptorSetLayoutBinding> vkBindings(spvSet->binding_count);

                for (size_t i = 0; i < spvSet->binding_count; ++i)
                {
                    vkBindings[i] = GetDescriptorBinding(spvSet->bindings[i]->name);
                }

                VkDescriptorSetLayoutCreateInfo layoutInfo{};
                layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                layoutInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
                layoutInfo.pBindings = vkBindings.data();

                VK_CHECK(vkCreateDescriptorSetLayout(mDevice->GetLogicDevice(), &layoutInfo, nullptr, &mDescriptorSetLayouts[setSlot]))
            }
        }
    }

    void GfxVulkanRasterShader::CreateDescriptorPool()
    {
        std::vector<VkDescriptorPoolSize> poolSizes;

        for (auto &[k, v] : mBindings)
        {
            bool alreadyExists = false;
            for (auto &poolSize : poolSizes)
            {
                if (v.descriptorType == poolSize.type)
                {
                    poolSize.descriptorCount++;
                    alreadyExists = true;
                    break;
                }
            }

            if (!alreadyExists)
            {
                VkDescriptorPoolSize newPoolSize;
                newPoolSize.type = v.descriptorType;
                newPoolSize.descriptorCount = 1;
                poolSizes.emplace_back(newPoolSize);
            }
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(mDescriptorSetLayouts.size());

        VK_CHECK(vkCreateDescriptorPool(mDevice->GetLogicDevice(), &poolInfo, nullptr, &mDescriptorPool))
    }

    void GfxVulkanRasterShader::AllocateDescriptorSets()
    {
        mDescriptorSets.resize(mDescriptorSetLayouts.size());
        for (size_t i = 0; i < mDescriptorSetLayouts.size(); ++i)
        {
            if (mDescriptorSetLayouts[i] == nullptr)
                continue;

            VkDescriptorSetAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocInfo.descriptorPool = GetDescriptorPool();
            allocInfo.descriptorSetCount = 1;
            allocInfo.pSetLayouts = &mDescriptorSetLayouts[i];

            VK_CHECK(vkAllocateDescriptorSets(mDevice->GetLogicDevice(), &allocInfo, &mDescriptorSets[i]))
        }
    }

    void GfxVulkanRasterShader::DumpDescriptorWrites()
    {
        auto GetSetIndex = [&](std::string_view name) -> uint32_t
        {
            for (size_t i = 0; i < 5; ++i)
            {
                for (const auto &spvBinding : mReflectedData[i].descriptorBindings)
                {
                    if (name == spvBinding->name)
                    {
                        return spvBinding->set;
                    }
                }
            }
            return 4096;
        };

        for (const auto &[k, v] : mBindings)
        {
            VkWriteDescriptorSet write = {};

            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = mDescriptorSets[GetSetIndex(k)];
            write.dstBinding = v.binding;
            write.dstArrayElement = 0;
            write.descriptorType = v.descriptorType;
            write.descriptorCount = v.descriptorCount;

            mWrites[k] = write;
        }
    }

    void GfxVulkanRasterShader::CreatePipelineLayout()
    {
        auto layouts = GetDescriptorSetLayoutList();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = layouts.size();
        pipelineLayoutInfo.pSetLayouts = layouts.data();

        VkDevice device = mDevice->GetLogicDevice();

        VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &mPipelineLayout))
    }

    std::vector<VkWriteDescriptorSet> GfxVulkanRasterShader::GetWrites()
    {
        std::vector<VkWriteDescriptorSet> result;
        for (auto [k, v] : mWrites)
        {
            result.emplace_back(v);
        }

        return result;
    }

    VkShaderStageFlagBits GfxVulkanRasterShader::GetShaderStageFlag(size_t idx)
    {
        switch (idx)
        {
        case 0:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case 1:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case 2:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case 3:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case 4:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        }
    }

    void GfxVulkanRasterShader::MarkDirty()
    {
        mIsDirty = true;
    }
}
#include "IGfxBuffer.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "VK/GfxVulkanBufferCommon.h"
namespace CynicEngine
{
    template <typename VertexType>
    inline VertexBuffer<VertexType>::VertexBuffer(IGfxDevice *device, const std::vector<VertexType> &vertices)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            mGfxBuffer.reset(GfxVulkanBufferCommon::CreateVertexBuffer(device, vertices));
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
    }

    template <typename VertexType>
    inline VertexBuffer<VertexType>::~VertexBuffer()
    {
        mGfxBuffer.reset(nullptr);
    }

    template <typename DataType>
    inline UniformBuffer<DataType>::UniformBuffer(IGfxDevice *device, const DataType &data)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            mGfxBuffer.reset(GfxVulkanBufferCommon::CreateUniformBuffer(device, data));
            break;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
    }

    template <typename DataType>
    inline UniformBuffer<DataType>::~UniformBuffer()
    {
        mGfxBuffer.reset(nullptr);
    }

    template <typename DataType>
    inline void UniformBuffer<DataType>::SetData(const DataType &data)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            GfxVulkanBufferCommon::SetCpuBufferData(mGfxBuffer.get(), sizeof(DataType), (void *)&(data));
            break;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
    }

}
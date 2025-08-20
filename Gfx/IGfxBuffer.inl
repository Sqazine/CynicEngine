#include "IGfxBuffer.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "VK/GfxVulkanBufferCommon.h"
namespace CynicEngine
{
    template <typename VertexType>
    inline IGfxVertexBuffer *IGfxVertexBuffer<VertexType>::Create(IGfxDevice *device, const std::vector<VertexType> &vertices)
    {
        IGfxVertexBuffer<VertexType> *vertexBuffer = new IGfxVertexBuffer<VertexType>();
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            vertexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateVertexBuffer(device, vertices));
            return vertexBuffer;
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
    inline IGfxUniformBuffer *IGfxUniformBuffer<DataType>::Create(IGfxDevice *device, const DataType &data)
    {
        IGfxUniformBuffer *uniformBuffer = new IGfxUniformBuffer();
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            uniformBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateUniformBuffer(device, data));
            return uniformBuffer;
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
    inline void IGfxUniformBuffer<DataType>::SetData(const DataType &data)
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
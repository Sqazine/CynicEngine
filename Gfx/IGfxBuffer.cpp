#include "IGfxBuffer.h"
#include "Config/AppConfig.h"
#include "Config/GfxConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanBufferCommon.h"
namespace CynicEngine
{
    IGfxVertexBuffer *IGfxVertexBuffer::Create(IGfxDevice *device, const IGfxBufferDesc &desc)
    {
        IGfxVertexBuffer *vertexBuffer = new IGfxVertexBuffer();
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            vertexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateVertexBuffer(device, desc));
            return vertexBuffer;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
        return nullptr; // for avoiding compiler warning
    }

    IGfxUniformBuffer *IGfxUniformBuffer::Create(IGfxDevice *device, const IGfxBufferDesc &desc)
    {
        IGfxUniformBuffer *uniformBuffer = new IGfxUniformBuffer();
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            uniformBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateUniformBuffer(device, desc));
            return uniformBuffer;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
        return nullptr; // for avoiding compiler warning
    }

    void IGfxUniformBuffer::SetData(const IGfxBufferDesc &desc)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            auto vulkanGfxBuffer = static_cast<GfxVulkanBuffer *>(mGfxBuffer.get());
            GfxVulkanBufferCommon::SetCpuBufferData(vulkanGfxBuffer, desc.bufferSize, desc.data);
            break;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
            break;
        }
    }
    
    IGfxIndexBuffer *IGfxIndexBuffer::Create(IGfxDevice *device, const IGfxBufferDesc &desc)
    {
        IGfxIndexBuffer *indexBuffer = new IGfxIndexBuffer();
        indexBuffer->mElementCount = desc.bufferSize / desc.elementSize;

        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            indexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateIndexBuffer(device, desc));
            return indexBuffer;
        }
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
        return nullptr; // for avoiding compiler warning
    }
}
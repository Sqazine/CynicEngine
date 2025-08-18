#include "IGfxBuffer.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanBufferCommon.h"
namespace CynicEngine
{
    IndexBuffer::IndexBuffer(IGfxDevice *device,const std::vector<uint32_t> &indices)
        : mElementCount(indices.size())
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            mGfxBuffer.reset(GfxVulkanBufferCommon::CreateIndexBuffer(device, indices));
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

    IndexBuffer::~IndexBuffer()
    {
        mGfxBuffer.reset(nullptr);
    }
}
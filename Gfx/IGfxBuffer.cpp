#include "IGfxBuffer.h"
#include "Config/AppConfig.h"
#include "Config/GfxConfig.h"
#include "Logger/Logger.h"
#include "Core/Marco.h"
#include "Gfx/VK/GfxVulkanBufferCommon.h"
namespace CynicEngine
{
    IGfxIndexBuffer *IGfxIndexBuffer::Create(IGfxDevice *device, const std::vector<uint32_t> &indices)
    {
        IGfxIndexBuffer* indexBuffer = new IGfxIndexBuffer();
        indexBuffer->mElementCount = indices.size();

        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
        {
            indexBuffer->mGfxBuffer.reset(GfxVulkanBufferCommon::CreateIndexBuffer(device, indices));
            return indexBuffer;
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
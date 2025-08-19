#include "IGfxPipeline.h"
#include "Config/AppConfig.h"
#include "Config/GfxConfig.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanPipeline.h"
namespace CynicEngine
{
    IGfxRasterPipeline::IGfxRasterPipeline(IGfxRasterShader *shader)
        : mShader(shader)
    {
    }
    
    IGfxRasterPipeline *IGfxRasterPipeline::Create(IGfxDevice *device, const IGfxVertexDesc &vertexDesc, IGfxRasterShader *shader)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanRasterPipeline(device, vertexDesc, shader);
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: %d"), static_cast<int>(gfxConfig.backend));
        return nullptr;
    }
}
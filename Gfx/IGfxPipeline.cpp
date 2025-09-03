#include "IGfxPipeline.h"
#include "Config/AppConfig.h"
#include "Config/GfxConfig.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanPipeline.h"
namespace CynicEngine
{
    IGfxRasterPipeline::IGfxRasterPipeline(const IGfxRasterPipelineDesc &pipelineDesc)
        : mPipelineDesc(pipelineDesc)
    {
    }

    IGfxRasterPipeline *IGfxRasterPipeline::Create(IGfxDevice *device, const IGfxRasterPipelineDesc &pipelineDesc)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanRasterPipeline(device, pipelineDesc);
        case GfxBackend::D3D12:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Not implemented D3D12 device creation yet"));
            break;
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
            break;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unreachable GfxBackend: {}"), static_cast<int>(gfxConfig.backend));
        return nullptr;
    }
}
#include "IGfxShader.h"
#include "Config/AppConfig.h"
#include "Logger/Logger.h"
#include "Gfx/VK/GfxVulkanShader.h"
namespace CynicEngine
{
    IGfxRasterShader *IGfxRasterShader::Create(IGfxDevice *device,
                                                        std::string_view vertContent,
                                                        std::string_view fragContent,
                                                        std::string_view tessCtrlContent,
                                                        std::string_view tessEvalContent,
                                                        std::string_view geomContent)
    {
        const GfxConfig &gfxConfig = AppConfig::GetInstance().GetGfxConfig();
        switch (gfxConfig.backend)
        {
        case GfxBackend::VULKAN:
            return new GfxVulkanRasterShader(device, vertContent, fragContent, tessCtrlContent, tessEvalContent, geomContent);
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
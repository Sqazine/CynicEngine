#include "IGfxCommon.h"
#include "Logger/Logger.h"
namespace CynicEngine
{
    uint8_t GetFormatByteCount(IGfxFormat format)
    {
        switch (format)
        {
        case IGfxFormat::R8_UNORM:
            return 1;
        case IGfxFormat::R8G8_UNORM:
            return 2;
        case IGfxFormat::R8G8B8_UNORM:
            return 3;
        case IGfxFormat::R8G8B8A8_UNORM:
        case IGfxFormat::R8G8B8A8_SRGB:
        case IGfxFormat::D32_SFLOAT:
        case IGfxFormat::D24_UNORM_S8_UINT:
            return 4;
        case IGfxFormat::R32G32_SFLOAT:
            return 8;
        case IGfxFormat::R32G32B32_SFLOAT:
            return 12;
        case IGfxFormat::R32G32B32A32_SFLOAT:
            return 16;
        case IGfxFormat::D32_SFLOAT_S8_UINT:
            return 5;
        }

        CYNIC_ENGINE_LOG_ERROR(TEXT("Unknown format!"));
    }
}
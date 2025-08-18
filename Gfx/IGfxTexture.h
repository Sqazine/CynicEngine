#pragma once
#include "IGfxDevice.h"
#include "IGfxCommon.h"
namespace CynicEngine
{
    struct GfxTextureDesc
    {
        void *data{nullptr};

        uint32_t width{0};
        uint32_t height{0};
        uint32_t mipLevels{0};

        Format format{Format::R8G8B8A8_SRGB};
        uint8_t sampleCount{1};

        Filter magFilter{Filter::NEAREST};
        Filter minFilter{Filter::NEAREST};
        AddressMode addressModeU{AddressMode::REPEAT};
        AddressMode addressModeV{AddressMode::REPEAT};
        AddressMode addressModeW{AddressMode::REPEAT};
    };

    class IGfxTexture
    {
    public:
        IGfxTexture(const GfxTextureDesc &desc);
        virtual ~IGfxTexture() = default;

        IGfxTexture *Create(IGfxDevice *device, const GfxTextureDesc &desc);

    protected:
        GfxTextureDesc mDesc;
    };
}
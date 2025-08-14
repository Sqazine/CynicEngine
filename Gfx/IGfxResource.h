#pragma once
#include "IGfxDevice.h"
namespace CynicEngine
{
    enum class Format
    {
        R8G8B8A8_SRGB,
        B8G8R8A8_SRGB,
        B8G8R8A8_UNORM,
        
        R32G32B32_SFLOAT,
        R32G32_SFLOAT,

        D32_SFLOAT,
        D32_SFLOAT_S8_UINT,
        D24_UNORM_S8_UINT
    };

    enum class Filter
    {
        NEAREST,
        LINEAR
    };

    enum class AddressMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
    };

    enum class IndexType
    {
        UINT16,
        UINT32
    };

    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;

        IGfxBuffer *Create(IGfxDevice *device);
    };

    struct GfxTextureDesc
    {
        void *data{nullptr};

        uint32_t width{0};
        uint32_t height{0};
        uint32_t mipLevels{0};

        Format format{Format::R8G8B8A8_SRGB} ;
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
#pragma once
#include "IGfxCommon.h"
#include "Math/Vector4.h"
namespace CynicEngine
{
    struct GfxTextureDesc
    {
        void *data{nullptr};

        uint32_t width{0};
        uint32_t height{0};
        uint32_t mipLevelCount{1};

        IGfxFormat format{IGfxFormat::R8G8B8A8_SRGB};
        uint8_t sampleCount{1};

        IGfxFilter magFilter{IGfxFilter::NEAREST};
        IGfxFilter minFilter{IGfxFilter::NEAREST};
        IGfxAddressMode addressModeU{IGfxAddressMode::REPEAT};
        IGfxAddressMode addressModeV{IGfxAddressMode::REPEAT};
        IGfxAddressMode addressModeW{IGfxAddressMode::REPEAT};
    };

    class IGfxDevice;
    class IGfxTexture
    {
    public:
        IGfxTexture(const GfxTextureDesc &desc);
        virtual ~IGfxTexture() = default;

        static IGfxTexture *Create(IGfxDevice *device, const GfxTextureDesc &desc);
        const GfxTextureDesc &GetDesc() const { return mDesc; }

        uint32_t GetMipLevelCount() const { return mDesc.mipLevelCount; }

    protected:
        GfxTextureDesc mDesc;
    };

    struct GfxDepthStencilClearValue
    {
        float depth;
        uint32_t stencil;
    };

    union GfxClearValue
    {
        Vector4f color;
        GfxDepthStencilClearValue depthStencil;
    };

    struct GfxTextureAttachment
    {
        IGfxTexture *texture{nullptr};
        AttachmentLoadOp loadOp{AttachmentLoadOp::DONT_CARE};
        AttachmentStoreOp storeOp{AttachmentStoreOp::DONT_CARE};
        GfxClearValue clearValue{Vector4f::ZERO};
    };
}
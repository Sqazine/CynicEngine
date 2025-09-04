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

        GfxFormat format{GfxFormat::R8G8B8A8_SRGB};
        uint8_t sampleCount{1};

        GfxFilter magFilter{GfxFilter::NEAREST};
        GfxFilter minFilter{GfxFilter::NEAREST};
        GfxAddressMode addressModeU{GfxAddressMode::REPEAT};
        GfxAddressMode addressModeV{GfxAddressMode::REPEAT};
        GfxAddressMode addressModeW{GfxAddressMode::REPEAT};
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
        GfxAttachmentLoadOp loadOp{GfxAttachmentLoadOp::DONT_CARE};
        GfxAttachmentStoreOp storeOp{GfxAttachmentStoreOp::DONT_CARE};
        GfxClearValue clearValue{Vector4f::ZERO};
    };
}
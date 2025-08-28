#pragma once
#include "Platform/Window.h"
#include "IGfxTexture.h"
namespace CynicEngine
{
    class IGfxDevice;
    class IGfxCommandBuffer;
    class IGfxSwapChain
    {
    public:
        IGfxSwapChain(Window *window);
        virtual ~IGfxSwapChain() = default;

        static IGfxSwapChain *Create(IGfxDevice *device, Window *window);

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual IGfxCommandBuffer *GetCurrentBackCommandBuffer() const = 0;

        virtual uint8_t GetBackBufferCount() const = 0;

        Window *GetWindow() const { return mWindow; }
        float GetAspect() const { return mWindow->GetAspect(); }

        const GfxTextureAttachment &GetColorAttachment() { return mColorAttachment; }
        const GfxTextureAttachment &GetDepthAttachment() { return mDepthAttachment; }

        virtual void SetColorAttachmentLoadOp(AttachmentLoadOp op);
        virtual AttachmentLoadOp GetColorAttachmentLoadOp() const;

        virtual void SetColorAttachmentStoreOp(AttachmentStoreOp op);
        virtual AttachmentStoreOp GetColorAttachmentStoreOp() const;

        virtual void SetDepthAttachmentLoadOp(AttachmentLoadOp op);
        virtual AttachmentLoadOp GetDepthAttachmentLoadOp() const;

        virtual void SetDepthAttachmentStoreOp(AttachmentStoreOp op);
        virtual AttachmentStoreOp GetDepthAttachmentStoreOp() const;

        virtual void SetClearColor(const Vector4f& color) =0 ;

    protected:
        Window *mWindow;
        GfxTextureAttachment mColorAttachment;
        GfxTextureAttachment mDepthAttachment;
    };
}
#pragma once
#include "IGfxDevice.h"
#include "IGfxCommon.h"
#include <vector>
namespace CynicEngine
{
    struct IGfxBufferDesc
    {
        size_t elementSize;
        size_t bufferSize;
        void *data{nullptr};
    };
    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;
    };

    class IGfxVertexBuffer
    {
    public:
        IGfxVertexBuffer() = default;
        ~IGfxVertexBuffer() { mGfxBuffer.reset(nullptr); }

        static IGfxVertexBuffer *Create(IGfxDevice *device, const IGfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    enum class IGfxIndexType
    {
        UINT16,
        UINT32
    };

    class IGfxIndexBuffer
    {
    public:
        IGfxIndexBuffer() = default;
        ~IGfxIndexBuffer() { mGfxBuffer.reset(nullptr); }

        static IGfxIndexBuffer *Create(IGfxDevice *device, const IGfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }
        IGfxIndexType GetIndexType() const { return mIndexType; }
        size_t GetElementCount() const { return mElementCount; }
    protected:
        size_t mElementCount;
        IGfxIndexType mIndexType{IGfxIndexType::UINT32};

        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    class IGfxUniformBuffer
    {
    public:
        IGfxUniformBuffer() = default;
        ~IGfxUniformBuffer() { mGfxBuffer.reset(nullptr); }

        static IGfxUniformBuffer *Create(IGfxDevice *device, const IGfxBufferDesc &desc);

        void SetData(const IGfxBufferDesc &desc);

        IGfxBuffer *GetGfxBuffer() const { return mGfxBuffer.get(); }

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };
}
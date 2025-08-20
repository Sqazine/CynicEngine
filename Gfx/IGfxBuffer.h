#pragma once
#include "IGfxDevice.h"
#include "IGfxCommon.h"
#include <vector>
namespace CynicEngine
{
    class IGfxBuffer
    {
    public:
        IGfxBuffer() = default;
        virtual ~IGfxBuffer() = default;
    };

    template <typename VertexType>
    class IGfxVertexBuffer
    {
    public:
        IGfxVertexBuffer() = default;
        ~IGfxVertexBuffer() { mGfxBuffer.reset(nullptr); }

        static IGfxVertexBuffer *Create(IGfxDevice *device, const std::vector<VertexType> &vertices);

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

        static IGfxIndexBuffer *Create(IGfxDevice *device, const std::vector<uint32_t> &indices);

    protected:
        size_t mElementCount;
        IGfxIndexType mIndexType{IGfxIndexType::UINT32};

        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    template <typename DataType>
    class IGfxUniformBuffer
    {
    public:
        IGfxUniformBuffer() = default;
        ~IGfxUniformBuffer() { mGfxBuffer.reset(nullptr); }

        static IGfxUniformBuffer *Create(IGfxDevice *device, const DataType &data);

        void SetData(const DataType &data);

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };
}
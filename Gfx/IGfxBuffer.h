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
    class VertexBuffer
    {
    public:
        VertexBuffer(IGfxDevice *device, const std::vector<VertexType> &vertices);
        ~VertexBuffer();

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    enum class IndexType
    {
        UINT16,
        UINT32
    };

    class IndexBuffer
    {
    public:
        IndexBuffer(IGfxDevice *device,const std::vector<uint32_t> &indices);
        ~IndexBuffer();

    protected:
        size_t mElementCount;
        IndexType mIndexType{IndexType::UINT32};

        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };

    template <typename DataType>
    class UniformBuffer
    {
    public:
        UniformBuffer(IGfxDevice *device,const DataType &data);
        ~UniformBuffer();

        void SetData(const DataType &data);

    protected:
        std::unique_ptr<IGfxBuffer> mGfxBuffer;
    };
}
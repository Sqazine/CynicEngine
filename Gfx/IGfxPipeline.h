#pragma once
#include "Gfx/IGfxCommon.h"
#include "Gfx/IGfxShader.h"
namespace CynicEngine
{
    struct GfxVertexAttribute
    {
        std::string_view name;
        GfxFormat format;
        size_t offset;
    };

    struct GfxVertexBinding
    {
        uint32_t bindingPoint;
        size_t size;
        GfxVertexInputType vertexInputType;
        std::vector<GfxVertexAttribute> attribs;
    };

    struct GfxRasterPipelineStateDesc
    {
        IGfxRasterShader *shader;
        GfxVertexBinding vertexBinding;
        GfxPrimitiveTopology primitiveTopology{GfxPrimitiveTopology::TRIANGLE_LIST};
        bool primitiveRestartEnable{false};
        GfxFrontFace frontFace{GfxFrontFace::COUNTER_CLOCKWISE};
        GfxPolygonMode polygonMode{GfxPolygonMode::FILL};
        float lineWidth{1.0f};
        GfxCullMode cullMode{GfxCullMode::NONE};
    };
    class IGfxRasterPipeline
    {
    public:
        IGfxRasterPipeline(const GfxRasterPipelineStateDesc &pipelineState);
        virtual ~IGfxRasterPipeline() = default;

        static IGfxRasterPipeline *Create(IGfxDevice *device, const GfxRasterPipelineStateDesc &pipelineState);

        IGfxRasterShader *GetShader() const { return mPipelineStateDesc.shader; }

    protected:
        GfxRasterPipelineStateDesc mPipelineStateDesc;
    };
}
#pragma once
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxVertexBinding.h"
namespace CynicEngine
{
    struct IGfxRasterPipelineDesc
    {
        IGfxVertexBinding vertexBinding;
        IGfxRasterShader* shader;
    };
    class IGfxRasterPipeline
    {
    public:
        IGfxRasterPipeline(const IGfxRasterPipelineDesc & pipelineDesc);
        virtual ~IGfxRasterPipeline() = default;

        static IGfxRasterPipeline *Create(IGfxDevice *device, const IGfxRasterPipelineDesc& pipelineDesc);

        IGfxRasterShader *GetShader() const { return mPipelineDesc.shader; }

    protected:
        IGfxRasterPipelineDesc mPipelineDesc;
    };
}
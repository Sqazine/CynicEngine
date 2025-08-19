#pragma once
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxVertexDesc.h"
namespace CynicEngine
{
    class IGfxRasterPipeline
    {
    public:
        IGfxRasterPipeline(IGfxRasterShader *shader);
        ~IGfxRasterPipeline() = default;

        static IGfxRasterPipeline *Create(IGfxDevice *device,const IGfxVertexDesc& vertexDesc,IGfxRasterShader *shader);

    protected:
        IGfxRasterShader *mShader;
    };
}
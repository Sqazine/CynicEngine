#pragma once
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxVertexDesc.h"
namespace CynicEngine
{
    class IGfxRasterPipeline
    {
    public:
        IGfxRasterPipeline(IGfxRasterShader *shader);
        virtual ~IGfxRasterPipeline() = default;

        static IGfxRasterPipeline *Create(IGfxDevice *device,const IGfxVertexDesc& vertexDesc,IGfxRasterShader *shader);

        IGfxRasterShader *GetShader() const { return mShader; }
    protected:
        IGfxRasterShader *mShader;
    };
}
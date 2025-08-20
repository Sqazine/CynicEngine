#pragma once
#include <memory>
#include "Render/Renderer.h"
#include "Render/Mesh.h"
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxPipeline.h"
namespace CynicEngine
{
    class MeshDrawPass
    {
    public:
        void Init();
        void Execute();

        Mesh mMesh;
        std::unique_ptr<IGfxRasterShader> mShader;
        std::unique_ptr<IGfxRasterPipeline> mRasterPipeline;
    };

    void AddMeshDrawPass(Renderer *renderer);
}
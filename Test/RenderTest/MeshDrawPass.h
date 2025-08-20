#pragma once
#include "Render/Renderer.h"
namespace CynicEngine
{
    class MeshDrawPass
    {
    public:
        void Init();
        void Execute();

        Mesh mMesh;
        std::unique_ptr<RasterShader> mShader;
        std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
    };

    void AddMeshDrawPass(Renderer *renderer);
}
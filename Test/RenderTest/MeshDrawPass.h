#pragma once
#include <memory>
#include "Render/Renderer.h"
#include "Render/Mesh.h"
#include "Render/Camera.h"
#include "Gfx/IGfxShader.h"
#include "Gfx/IGfxPipeline.h"
namespace CynicEngine
{
    class MeshDrawPass
    {
    public:
        void Init();
        void Execute();

        struct MeshUniformData
        {
            alignas(16) Matrix4f model;
        };

        MeshUniformData mMeshUniformData;

        std::unique_ptr<Camera> mCamera;
        std::unique_ptr<Mesh> mMesh;
        std::unique_ptr<IGfxUniformBuffer> mMeshUniformDataBuffer;
        std::unique_ptr<IGfxRasterShader> mShader;
        std::unique_ptr<IGfxRasterPipeline> mRasterPipeline;
    };

    void AddMeshDrawPass(Renderer *renderer);
}
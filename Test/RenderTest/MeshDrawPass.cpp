#include "MeshDrawPass.h"
#include "Core/IO.h"
namespace CynicEngine
{
    void MeshDrawPass::Init()
    {
        Vertex v0{};
        v0.position = Vector3f(-1.0, 1.0, 0.0);
        v0.color = Vector4f(1.0, 0.0, 0.0, 1.0);
        v0.texCoord0 = Vector2f(0.0, 0.0);

        Vertex v1{};
        v1.position = Vector3f(1.0, 1.0, 0.0);
        v1.color = Vector4f(0.0, 1.0, 0.0, 1.0);
        v1.texCoord0 = Vector2f(1.0, 0.0);

        Vertex v2{};
        v2.position = Vector3f(0.0, -1.0, 0.0);
        v2.color = Vector4f(0.0, 0.0, 1.0, 1.0);
        v2.texCoord0 = Vector2f(0.5, 1.0);

        std::vector<uint32_t> indices = {0, 1, 2};

        mMesh.SetVertices({v0, v1, v2});
        mMesh.SetIndices(indices);

        std::string shaderCompile;
        shaderCompile = "slangc.exe " SHADER_DIR "meshDrawPass.vert.slang" " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "meshDrawPass.vert.slang.spv" " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());
        shaderCompile = "slangc.exe " SHADER_DIR "meshDrawPass.frag.slang" " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "meshDrawPass.frag.slang.spv" " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());

        auto vertShaderContent = ReadFile(SHADER_DIR "meshDrawPass.vert.slang.spv");
        auto fragShaderContent = ReadFile(SHADER_DIR "meshDrawPass.frag.slang.spv");
        mShader.reset(IGfxRasterShader::Create(Renderer::GetGfxDevice(), vertShaderContent, fragShaderContent));
    }

    void MeshDrawPass::Execute()
    {
    }

    void AddMeshDrawPass(Renderer *renderer)
    {
        renderer->AddRenderTask<MeshDrawPass>(
            "MeshDrawPass",
            true,
            [&](MeshDrawPass &task, RenderTaskBuilder &builder)
            {
                task.Init();
            },
            [=](MeshDrawPass &task)
            {
                task.Execute();
            });
    }
}
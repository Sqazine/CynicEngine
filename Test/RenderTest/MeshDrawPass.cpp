#include "MeshDrawPass.h"
#include "Core/IO.h"
#include "Gfx/IGfxCommandBuffer.h"
namespace CynicEngine
{
    void MeshDrawPass::Init()
    {
        std::string shaderCompile;
        shaderCompile = "slangc.exe " SHADER_DIR "meshDrawPass.vert.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "meshDrawPass.vert.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());
        shaderCompile = "slangc.exe " SHADER_DIR "meshDrawPass.frag.slang"
                        " -profile sm_6_6+spirv_1_6 -target spirv -o " SHADER_DIR "meshDrawPass.frag.slang.spv"
                        " -emit-spirv-directly -fvk-use-entrypoint-name";
        system(shaderCompile.c_str());

        mCamera = std::make_unique<Camera>();
        mCamera->SetPosition(Vector3f(0.0f, 0.0f, 10.0f));
        mCamera->SetFovByDegree(60.0f);
        mMesh.reset(Mesh::CreateBuiltinMesh(MeshType::TRIANGLE));

        GfxTextureDesc textureDesc=ReadTexture(ASSETS_DIR "uv.png");
        mColorTexture.reset(IGfxTexture::Create(Renderer::GetGfxDevice(),textureDesc));

        IGfxBufferDesc desc{};
        desc.bufferSize = sizeof(MeshUniformData);
        desc.elementSize = sizeof(MeshUniformData);
        desc.data = &mMeshUniformData;
        mMeshUniformDataBuffer.reset(IGfxUniformBuffer::Create(Renderer::GetGfxDevice(),desc));

        auto vertShaderContent = ReadFile(SHADER_DIR "meshDrawPass.vert.slang.spv");
        auto fragShaderContent = ReadFile(SHADER_DIR "meshDrawPass.frag.slang.spv");
        mShader.reset(IGfxRasterShader::Create(Renderer::GetGfxDevice(), vertShaderContent, fragShaderContent));
        mRasterPipeline.reset(IGfxRasterPipeline::Create(Renderer::GetGfxDevice(), Vertex::GetVertexDesc(), mShader.get()));

        mShader->BindBuffer("cameraData", mCamera->GetRenderDataBuffer()->GetGfxBuffer());
        mShader->BindBuffer("meshUBO", mMeshUniformDataBuffer->GetGfxBuffer());
        mShader->BindTexture("texSampler", mColorTexture.get());
    }

    void MeshDrawPass::Execute()
    {
        auto swapChain = Renderer::GetGfxDevice()->GetSwapChain();
        swapChain->GetColorAttachment().loadOp=AttachmentLoadOp::CLEAR;
        swapChain->GetColorAttachment().storeOp=AttachmentStoreOp::STORE;
        
        swapChain->GetDepthAttachment().loadOp=AttachmentLoadOp::CLEAR;
        swapChain->GetDepthAttachment().storeOp=AttachmentStoreOp::STORE;

        auto cmdBuffer = Renderer::GetGfxDevice()->GetCurrentBackCommandBuffer();
        cmdBuffer
            ->BeginRenderPass(swapChain)
            ->BindRasterPipeline(mRasterPipeline.get())
            ->BindVertexBuffer(mMesh->GetVertexBuffer())
            ->BindIndexBuffer(mMesh->GetIndexBuffer())
            ->DrawIndexed(mMesh->GetIndexBuffer()->GetElementCount(), 1, 0, 0, 0)
            ->EndRenderPass();
    }

    void AddMeshDrawPass(Renderer *renderer)
    {
        renderer->AddRenderTask<MeshDrawPass>(
            "MeshDrawPass",
            true,
            [&](MeshDrawPass *task, RenderTaskBuilder &builder)
            {
                task->Init();
            },
            [=](MeshDrawPass *task)
            {
                task->Execute();
            });
    }
}
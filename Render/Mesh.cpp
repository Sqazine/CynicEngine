#include "Mesh.h"
#include <vector>
#include "Render/Renderer.h"
namespace CynicEngine
{
    void Mesh::SetVertices(const std::vector<Vertex> &vertices)
    {
        if (vertices == mVertices)
            return;

        mVertices = vertices;
        mVertexBuffer.reset(IGfxVertexBuffer<Vertex>::Create(Renderer::GetGfxDevice(), mVertices));
    }
    void Mesh::SetIndices(const std::vector<uint32_t> &indices)
    {
        if (indices == mIndices)
            return;

        mIndices = indices;
        mIndexBuffer.reset(IGfxIndexBuffer::Create(Renderer::GetGfxDevice(), mIndices));
    }
}
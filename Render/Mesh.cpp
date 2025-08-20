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

        IGfxBufferDesc gfxDesc{};
        gfxDesc.elementSize = sizeof(Vertex);
        gfxDesc.bufferSize = mVertices.size() * gfxDesc.elementSize;
        gfxDesc.data = mVertices.data();

        mVertexBuffer.reset(IGfxVertexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
    }
    void Mesh::SetIndices(const std::vector<uint32_t> &indices)
    {
        if (indices == mIndices)
            return;

        mIndices = indices;

        IGfxBufferDesc gfxDesc{};
        gfxDesc.elementSize = sizeof(uint32_t);
        gfxDesc.bufferSize = mIndices.size() * gfxDesc.elementSize;
        gfxDesc.data = mIndices.data();

        mIndexBuffer.reset(IGfxIndexBuffer::Create(Renderer::GetGfxDevice(), gfxDesc));
    }
}
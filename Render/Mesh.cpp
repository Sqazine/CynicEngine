#include "Mesh.h"
#include <vector>
#include "Render/Renderer.h"
namespace CynicEngine
{
    Mesh *Mesh::CreateBuiltinMesh(MeshType type)
    {
        switch (type)
        {
        case MeshType::QUAD:
            return CreateBuiltinQuad();
        case MeshType::CUBE:
            return CreateBuiltinCube();
        case MeshType::SPHERE:
            return CreateBuiltinSphere();
        case MeshType::CAPSULE:
            return CreateBuiltinCapsule();
        case MeshType::CYLINDER:
            return CreateBuiltinCylinder();
        default:
            CYNIC_ENGINE_LOG_ERROR(TEXT("Unsupported mesh type!"));
            return nullptr;
        }
    }
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

    Mesh *Mesh::CreateBuiltinQuad()
    {
        return nullptr; // TODO: Implementation not ready
    }

    Mesh *Mesh::CreateBuiltInCube()
    {
        return nullptr; // TODO: Implementation not ready
    }

    Mesh *Mesh::CreateBuiltInSphere()
    {
        return nullptr; // TODO: Implementation not ready
    }

    Mesh *Mesh::CreateBuiltInCapsule()
    {
        return nullptr; // TODO: Implementation not ready
    }

    Mesh *Mesh::CreateBuiltInCylinder()
    {
        return nullptr; // TODO: Implementation not ready
    }

}
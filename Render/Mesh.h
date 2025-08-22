#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Gfx/IGfxVertexDesc.h"
#include "Gfx/IGfxBuffer.h"
namespace CynicEngine
{
    struct Vertex
    {
        alignas(16) Vector3f position;
        alignas(16) Vector3f normal;
        alignas(16) Vector4f color;
        alignas(8) Vector2f texCoord0;

        bool operator==(const Vertex &other) const
        {
            return position == other.position && color == other.color && texCoord0 == other.texCoord0;
        }

        static IGfxVertexDesc GetVertexDesc()
        {
            static IGfxVertexDesc vertexDesc;
            static bool isInit = false;

            if (!isInit)
            {
                isInit = true;

                vertexDesc.bindingPoint = 0;
                vertexDesc.size = sizeof(Vertex);

                IGfxVertexAttrib attrib;
                attrib.name = "POSITION";
                attrib.format = IGfxFormat::R32G32B32_SFLOAT;
                attrib.offset = offsetof(Vertex, position);
                vertexDesc.attribs.emplace_back(attrib);

                attrib.name = "COLOR";
                attrib.format = IGfxFormat::R32G32B32A32_SFLOAT;
                attrib.offset = offsetof(Vertex, color);
                vertexDesc.attribs.emplace_back(attrib);

                attrib.name = "TEXCOORD";
                attrib.format = IGfxFormat::R32G32_SFLOAT;
                attrib.offset = offsetof(Vertex, texCoord0);
                vertexDesc.attribs.emplace_back(attrib);
            }
            return vertexDesc;
        }
    };

    enum class MeshType
    {
        QUAD,
        CUBE,
        SPHERE,
        CAPSULE,
        CYLINDER,
    };

    class Mesh
    {
    public:
        Mesh() = default;
        ~Mesh() = default;

        static Mesh* CreateBuiltinMesh(MeshType type);

        void SetVertices(const std::vector<Vertex> &vertices);
        void SetIndices(const std::vector<uint32_t> &indices);

        const std::vector<Vertex> &GetVertices() const { return mVertices; }
        const std::vector<uint32_t> &GetIndices() const { return mIndices; }

        const IGfxVertexBuffer *GetVertexBuffer() const { return mVertexBuffer.get(); }

        bool HasIndices() const { return !mIndices.empty(); }
        const IGfxIndexBuffer *GetIndexBuffer() const { return mIndexBuffer.get(); }

    private:
        static Mesh* CreateBuiltinQuad();
        static Mesh* CreateBuiltInCube();
        static Mesh* CreateBuiltInSphere();
        static Mesh* CreateBuiltInCapsule();
        static Mesh* CreateBuiltInCylinder();

        std::vector<Vertex> mVertices{};
        std::vector<uint32_t> mIndices{};

        std::unique_ptr<IGfxVertexBuffer> mVertexBuffer;
        std::unique_ptr<IGfxIndexBuffer> mIndexBuffer;
    };
}
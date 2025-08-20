#pragma once
#include "IGfxBuffer.h"
#include "IGfxTexture.h"
#include "Renderer.h"

namespace CynicEngine
{
    template <>
    inline std::unique_ptr<IGfxVertexBuffer> Realize(const std::vector<Vertex> &vertices)
    {
        return std::make_unique<VertexBuffer>(vertices);
    }

    template <>
    inline std::unique_ptr<IGfxIndexBuffer> Realize(const std::vector<uint32_t> &indices)
    {
        return std::make_unique<IndexBuffer>(indices);
    }

    template <>
    inline std::unique_ptr<IGfxUniformBuffer> Realize(const UniformBufferDesc &desc)
    {
        return std::make_unique<UniformBuffer>(desc);
    }
}
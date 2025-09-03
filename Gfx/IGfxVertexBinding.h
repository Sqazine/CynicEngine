#pragma once
#include <vector>
#include <cstdint>
#include <string_view>
#include "IGfxCommon.h"
namespace CynicEngine
{
    struct IGfxVertexAttribute
    {
        std::string_view name;
        IGfxFormat format;
        size_t offset;
    };

    enum class IGfxVertexInputType
    {
        PER_VERTEX,
        PER_INSTANCE,
    };

    struct IGfxVertexBinding
    {
        uint32_t bindingPoint;
        size_t size;
        IGfxVertexInputType vertexInputType;
        std::vector<IGfxVertexAttribute> attribs;
    };
}
#pragma once
#include <vector>
#include <cstdint>
#include <string_view>
#include "IGfxCommon.h"
namespace CynicEngine
{
    struct IGfxVertexAttrib
    {
        std::string_view name;
        IGfxFormat format;
        size_t offset;
    };

    struct IGfxVertexDesc
    {
        uint32_t bindingPoint;
        size_t size;
        std::vector<IGfxVertexAttrib> attribs;
    };
}
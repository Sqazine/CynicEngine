#pragma once
#include "IGfxDevice.h"
#include "IGfxTexture.h"
#include "IGfxBuffer.h"
namespace CynicEngine
{
    class IGfxRasterShader
    {
    public:
        IGfxRasterShader() = default;
        virtual ~IGfxRasterShader() = default;

        static IGfxRasterShader *Create(IGfxDevice *device,
                                                 std::string_view vertContent,
                                                 std::string_view fragContent,
                                                 std::string_view tessCtrlContent = "",
                                                 std::string_view tessEvalContent = "",
                                                 std::string_view geomContent = "");

        virtual IGfxRasterShader *BindBuffer(std::string_view name, const IGfxBuffer *buffer) = 0;
        virtual IGfxRasterShader *BindTexture(std::string_view name, const IGfxTexture *texture) = 0;

        virtual void Flush() = 0;
    };
}
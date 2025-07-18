#pragma once
#include "Gfx/IGfxResource.h"
#include "GfxVulkanObject.h"
#include "Gfx/IGfxDevice.h"
namespace CynicEngine
{
    class GfxVulkanBuffer : public GfxVulkanObject, public IGfxBuffer
    {
    public:
        GfxVulkanBuffer(IGfxDevice *device);
        ~GfxVulkanBuffer() override;
    };

    class GfxVulkanTexture : public GfxVulkanObject, public IGfxTexture
    {
    public:
        GfxVulkanTexture(IGfxDevice *device);
        ~GfxVulkanTexture() override;
    };
}
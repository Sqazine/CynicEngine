#include "GfxVulkanResource.h"

namespace CynicEngine
{
    GfxVulkanBuffer::GfxVulkanBuffer(IGfxDevice *device)
        : GfxVulkanObject(device)
    {
    }

    GfxVulkanBuffer::~GfxVulkanBuffer()
    {
    }

    GfxVulkanTexture::GfxVulkanTexture(IGfxDevice *device)
        : GfxVulkanObject(device)
    {
    }

    GfxVulkanTexture::~GfxVulkanTexture()
    {
    }
}
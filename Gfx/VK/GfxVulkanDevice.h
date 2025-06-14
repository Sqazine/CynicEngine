#pragma once
#include <vulkan/vulkan.hpp>
#include "Gfx/IGfxDevice.h"
class GfxVulkanDevice : public IGfxDevice
{
public:
    GfxVulkanDevice(const GfxDeviceDesc &desc);

private:
    void CreateInstance();

    vk::UniqueInstance mInstance;
};
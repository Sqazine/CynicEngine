#include "GfxVulkanDevice.h"

GfxVulkanDevice::GfxVulkanDevice(const GfxDeviceDesc &desc)
    : IGfxDevice(desc)
{
    CreateInstance();
}

void GfxVulkanDevice::CreateInstance()
{
    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName = "Cynic Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Cynic Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;

    mInstance = vk::createInstanceUnique(createInfo);
}
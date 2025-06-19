#pragma once
#include <vulkan/vulkan.h>
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanCommon.h"

namespace CynicEngine
{
	class GfxVulkanDevice : public IGfxDevice
	{
	public:
		GfxVulkanDevice(const GfxDeviceDesc &desc, const Window *window);
		~GfxVulkanDevice() override;

	private:
		void CreateInstance();
#ifndef NDEBUG
		void CreateDebugMessengerLayer();
#endif
		void EnumeratePhysicalDevices();

		std::vector<VkLayerProperties> mInstanceLayers;
		std::vector<VkExtensionProperties> mInstanceExtensions;
		VkInstance mInstance;

#ifndef NDEBUG
		const std::vector<const char *> mValidationInstanceLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_monitor",
		};

		const std::vector<const char *> mDebugRequiredInstanceExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif

	private:
		void CheckInstanceValidationLayersIsSatisfied();
		void CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions);

		VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
		std::vector<const char *> GetRequiredInstanceExtensions();
	};
}
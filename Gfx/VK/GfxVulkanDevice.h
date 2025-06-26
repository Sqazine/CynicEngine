#pragma once
#include <optional>
#include <vulkan/vulkan.h>
#include "Gfx/IGfxDevice.h"
#include "GfxVulkanCommon.h"

namespace CynicEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamilyIdx;
		std::optional<uint32_t> computeFamilyIdx;
		std::optional<uint32_t> transferFamilyIdx;
		std::optional<uint32_t> presentFamilyIdx;

		bool IsComplete() const
		{
			return graphicsFamilyIdx.has_value() &&
				   computeFamilyIdx.has_value() &&
				   transferFamilyIdx.has_value() &&
				   presentFamilyIdx.has_value();
		}

		bool IsSameFamilyIndex() const
		{
			return graphicsFamilyIdx.value() == computeFamilyIdx.value() && computeFamilyIdx.value() == transferFamilyIdx.value() && transferFamilyIdx.value() == presentFamilyIdx.value();
		}

		std::vector<uint32_t> IndexArray() const
		{
			return {graphicsFamilyIdx.value(), computeFamilyIdx.value(), transferFamilyIdx.value(), presentFamilyIdx.value()};
		}
	};

	struct PhysicalDeviceSpecification
	{
		VkPhysicalDevice handle;
		std::vector<VkExtensionProperties> deviceExtensions;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures features;
		VkPhysicalDeviceMemoryProperties memoryProperties;
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		QueueFamilyIndices queueFamilyIndices;
	};

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
		void CreateSurface();
		void EnumeratePhysicalDevices();
		void SelectPhysicalDevice();
		void CreateLogicDevice();
		void CreateQueues();

		std::vector<VkLayerProperties> mInstanceLayers;
		std::vector<VkExtensionProperties> mInstanceExtensions;
		VkInstance mInstance;

		VkSurfaceKHR mSurface;

#ifndef NDEBUG
		const std::vector<const char *> mValidationInstanceLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_monitor",
		};

		const std::vector<const char *> mDebugRequiredInstanceExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

		VkDebugUtilsMessengerEXT mDebugMessenger;
#endif
		std::vector<PhysicalDeviceSpecification> mPhysicalDeviceSpecificationList;
		size_t mSelectedPhysicalDeviceIndex = 0;

		const std::vector<const char *> mDeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		VkDevice mLogicDevice;

		std::unique_ptr<GfxVulkanGraphicsQueue> mGraphicsQueue;
		std::unique_ptr<GfxVulkanComputeQueue> mComputeQueue;
		std::unique_ptr<GfxVulkanTransferQueue> mTransferQueue;
		std::unique_ptr<GfxVulkanPresentQueue> mPresentQueue;

	private:
		void CheckInstanceValidationLayersIsSatisfied();
		void CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions);

		VkDebugUtilsMessengerCreateInfoEXT PopulateDebugMessengerCreateInfo();
		std::vector<const char *> GetRequiredInstanceExtensions();
		PhysicalDeviceSpecification EnumeratePhysicalDeviceSpecificationFor(VkPhysicalDevice device);

		void DumpPhysicalDeviceSpecifications();
	};
}
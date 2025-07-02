#include "GfxVulkanDevice.h"
#include <iostream>
#include "Version.h"
#include "Core/Logger.h"
#include "Platform/PlatformInfo.h"

namespace CynicEngine
{

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallbackFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
	{
		Logger::Kind loggerKind;

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			loggerKind = Logger::Kind::ERROR;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			loggerKind = Logger::Kind::WARN;
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			loggerKind = Logger::Kind::INFO;
			break;
		default:
			break;
		}

		STRING tags;

		switch (messageType)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			tags += TEXT("[GENERAL]");
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			tags += TEXT("[VALIDATION]");
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			tags += TEXT("[PERFORMANCE]");
			break;
		default:
			break;
		}

		switch (pCallbackData->pObjects->objectType)
		{
		case VK_OBJECT_TYPE_INSTANCE:
			tags += TEXT("[INSTANCE]");
			break;
		case VK_OBJECT_TYPE_PHYSICAL_DEVICE:
			tags += TEXT("[PHYSICAL_DEVICE]");
			break;
		case VK_OBJECT_TYPE_DEVICE:
			tags += TEXT("[DEVICE]");
			break;
		case VK_OBJECT_TYPE_QUEUE:
			tags += TEXT("[QUEUE]");
			break;
		case VK_OBJECT_TYPE_SEMAPHORE:
			tags += TEXT("[SEMAPHORE]");
			break;
		case VK_OBJECT_TYPE_COMMAND_BUFFER:
			tags += TEXT("[COMMAND_BUFFER]");
			break;
		case VK_OBJECT_TYPE_FENCE:
			tags += TEXT("[FENCE]");
			break;
		case VK_OBJECT_TYPE_DEVICE_MEMORY:
			tags += TEXT("[DEVICE_MEMORY]");
			break;
		case VK_OBJECT_TYPE_BUFFER:
			tags += TEXT("[BUFFER]");
			break;
		case VK_OBJECT_TYPE_IMAGE:
			tags += TEXT("[IMAGE]");
			break;
		case VK_OBJECT_TYPE_EVENT:
			tags += TEXT("[EVENT]");
			break;
		case VK_OBJECT_TYPE_QUERY_POOL:
			tags += TEXT("[QUERY_POOL]");
			break;
		case VK_OBJECT_TYPE_BUFFER_VIEW:
			tags += TEXT("[BUFFER_VIEW]");
			break;
		case VK_OBJECT_TYPE_IMAGE_VIEW:
			tags += TEXT("[IMAGE_VIEW]");
			break;
		case VK_OBJECT_TYPE_SHADER_MODULE:
			tags += TEXT("[SHADER_MODULE]");
			break;
		case VK_OBJECT_TYPE_PIPELINE_CACHE:
			tags += TEXT("[PIPELINE_CACHE]");
			break;
		case VK_OBJECT_TYPE_PIPELINE_LAYOUT:
			tags += TEXT("[PIPELINE_LAYOUT]");
			break;
		case VK_OBJECT_TYPE_RENDER_PASS:
			tags += TEXT("[RENDER_PASS]");
			break;
		case VK_OBJECT_TYPE_PIPELINE:
			tags += TEXT("[PIPELINE]");
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:
			tags += TEXT("[DESCRIPTOR_SET_LAYOUT]");
			break;
		case VK_OBJECT_TYPE_SAMPLER:
			tags += TEXT("[SAMPLER]");
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_POOL:
			tags += TEXT("[DESCRIPTOR_POOL]");
			break;
		case VK_OBJECT_TYPE_DESCRIPTOR_SET:
			tags += TEXT("[DESCRIPTOR_SET]");
			break;
		case VK_OBJECT_TYPE_FRAMEBUFFER:
			tags += TEXT("[FRAMEBUFFER]");
			break;
		case VK_OBJECT_TYPE_COMMAND_POOL:
			tags += TEXT("[COMMAND_POOL]");
			break;
		case VK_OBJECT_TYPE_SURFACE_KHR:
			tags += TEXT("[SURFACE_KHR]");
			break;
		case VK_OBJECT_TYPE_SWAPCHAIN_KHR:
			tags += TEXT("[SWAPCHAIN_KHR]");
			break;
		case VK_OBJECT_TYPE_DISPLAY_KHR:
			tags += TEXT("[DISPLAY_KHR]");
			break;
		case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:
			tags += TEXT("[DISPLAY_MODE_KHR]");
			break;
		default:
			break;
		}

		Logger::Log(loggerKind, TEXT("Vulkan Valication Layer {}:{}"), tags.c_str(), pCallbackData->pMessage);

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	GfxVulkanDevice::GfxVulkanDevice(const Window *window)
		: IGfxDevice(window)
	{
		CreateInstance();
#ifndef NDEBUG
		CreateDebugMessengerLayer();
#endif
		EnumeratePhysicalDevices();
		SelectPhysicalDevice();
		CreateLogicDevice();
		GetQueues();
	}

	GfxVulkanDevice::~GfxVulkanDevice()
	{
		vkDestroyDevice(mLogicDevice, nullptr);
#ifndef NDEBUG
		DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
		vkDestroyInstance(mInstance, nullptr);
	}

	void GfxVulkanDevice::CreateInstance()
	{
		{
			uint32_t count = 0;
			vkEnumerateInstanceLayerProperties(&count, nullptr);
			mInstanceLayers.resize(count);
			vkEnumerateInstanceLayerProperties(&count, mInstanceLayers.data());
		}
		{
			uint32_t count = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
			mInstanceExtensions.resize(count);
			vkEnumerateInstanceExtensionProperties(nullptr, &count, mInstanceExtensions.data());
		}

		VkApplicationInfo appInfo{};
		appInfo.pApplicationName = "Cynic Engine";			   //TODO: Get application name from config
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO: Get application version from config
		appInfo.pEngineName = "Cynic Engine";
		appInfo.engineVersion = CYNIC_ENGINE_VERSION_BINARY;
		appInfo.apiVersion = VK_API_VERSION_1_4;

		VkInstanceCreateInfo instanceCreateInfo{};
		ZeroVulkanStruct(instanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		instanceCreateInfo.pApplicationInfo = &appInfo;

#ifndef NDEBUG
		CheckInstanceValidationLayersIsSatisfied();
		auto debugInfo = PopulateDebugMessengerCreateInfo();

		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(mValidationInstanceLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = mValidationInstanceLayers.data();
		instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugInfo;
#else
		instanceCreateInfo.enabledLayerCount = 0;
		instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

		auto requiredInstanceExtensions = GetRequiredInstanceExtensions();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredInstanceExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions.data();

		vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
	}

#ifndef NDEBUG
	void GfxVulkanDevice::CreateDebugMessengerLayer()
	{
		auto debugInfo = PopulateDebugMessengerCreateInfo();
		CreateDebugUtilsMessengerEXT(mInstance, &debugInfo, nullptr, &mDebugMessenger);
	}
#endif

	void GfxVulkanDevice::EnumeratePhysicalDevices()
	{
		uint32_t count;
		vkEnumeratePhysicalDevices(mInstance, &count, nullptr);

		std::vector<VkPhysicalDevice> physicalDeviceHandleList(count);
		vkEnumeratePhysicalDevices(mInstance, &count, physicalDeviceHandleList.data());

		mPhysicalDeviceSpecificationList.resize(count);
		for (int32_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
		{
			mPhysicalDeviceSpecificationList[i] = EnumeratePhysicalDeviceSpecificationFor(physicalDeviceHandleList[i]);

			const auto &spec = mPhysicalDeviceSpecificationList[i];
		}

		DumpPhysicalDeviceSpecifications();
	}

	void GfxVulkanDevice::SelectPhysicalDevice()
	{
		if (mPhysicalDeviceSpecificationList.empty())
		{
			CYNIC_ENGINE_LOG_ERROR(TEXT("No Vulkan physical device found!"));
		}
		else if (mPhysicalDeviceSpecificationList.size() == 1)
		{
			mSelectedPhysicalDeviceIndex = 0;
			CYNIC_ENGINE_LOG_INFO(TEXT("Only one Vulkan physical device found, using it."));
		}
		else
		{
			for (size_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
			{
				const auto &spec = mPhysicalDeviceSpecificationList[i];
				if (spec.deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					mSelectedPhysicalDeviceIndex = i;
					break;
				}
			}
		}

		const auto &finalSelectedSpec = mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex];
		CYNIC_ENGINE_LOG_INFO(TEXT("Selected Vulkan physical device {}: {}, Type: {}, API Version: {}, Driver Version: {}"),
							  mSelectedPhysicalDeviceIndex, finalSelectedSpec.deviceProperties.deviceName, finalSelectedSpec.deviceProperties.deviceType,
							  finalSelectedSpec.deviceProperties.apiVersion, finalSelectedSpec.deviceProperties.driverVersion);
	}

	void GfxVulkanDevice::CreateLogicDevice()
	{
		const auto &finalSelectedSpec = mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex];

		const float queuePriority = 1.0f;
		VkDeviceQueueCreateInfo deviceQueueInfo;
		ZeroVulkanStruct(deviceQueueInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
		deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueInfo.queueCount = 1;
		deviceQueueInfo.pQueuePriorities = &queuePriority;

		VkDeviceCreateInfo deviceInfo;
		ZeroVulkanStruct(deviceInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
		deviceInfo.pNext = nullptr;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &deviceQueueInfo;
		deviceInfo.enabledExtensionCount = (uint32_t)mDeviceExtensions.size();
		deviceInfo.ppEnabledExtensionNames = mDeviceExtensions.data();
		deviceInfo.pEnabledFeatures = nullptr;

		VK_CHECK(vkCreateDevice(finalSelectedSpec.handle, &deviceInfo, nullptr, &mLogicDevice));
	}

	void GfxVulkanDevice::GetQueues()
	{
		const auto &finalSelectedSpec = mPhysicalDeviceSpecificationList[mSelectedPhysicalDeviceIndex];

		if (finalSelectedSpec.queueFamilyIndices.graphicsFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.graphicsFamilyIdx.value(), 0, &mGraphicsQueue);
		}
		if (finalSelectedSpec.queueFamilyIndices.computeFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.computeFamilyIdx.value(), 0, &mComputeQueue);
		}
		if (finalSelectedSpec.queueFamilyIndices.transferFamilyIdx.has_value())
		{
			vkGetDeviceQueue(mLogicDevice, finalSelectedSpec.queueFamilyIndices.transferFamilyIdx.value(), 0, &mTransferQueue);
		}
		else
		{
			CYNIC_ENGINE_LOG_ERROR(TEXT("No present queue found for the selected physical device!"));
		}
	}

	void GfxVulkanDevice::CheckInstanceValidationLayersIsSatisfied()
	{
		for (const char *layerName : mValidationInstanceLayers)
		{
			bool layerFound = false;

			for (const auto &layerProperties : mInstanceLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				CYNIC_ENGINE_LOG_ERROR(TEXT("Missing Vulkan Validation Layer:{}"), layerName);
			}
		}
	}

	void GfxVulkanDevice::CheckRequiredInstanceExtensionsIsSatisfied(const std::vector<const char *> &extensions)
	{
		for (const char *extensionName : extensions)
		{
			bool isFound = false;

			for (const auto &extensionProperties : mInstanceExtensions)
			{
				if (strcmp(extensionName, extensionProperties.extensionName) == 0)
				{
					isFound = true;
					break;
				}
			}

			if (!isFound)
			{
				CYNIC_ENGINE_LOG_ERROR(TEXT("Missing Vulkan Instance Extension:%s"), extensionName);
			}
		}
	}

	VkDebugUtilsMessengerCreateInfoEXT GfxVulkanDevice::PopulateDebugMessengerCreateInfo()
	{
		VkDebugUtilsMessengerCreateInfoEXT info = {};
		ZeroVulkanStruct(info, VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT);
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = DebugCallbackFunc;

		return info;
	}

	std::vector<const char *> GfxVulkanDevice::GetRequiredInstanceExtensions()
	{
		std::vector<const char *> result = PlatformInfo::GetInstance().GetVulkanPlatformInfo()->GetWindowInstanceExtension();
		result.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#ifndef NDEBUG
		result.insert(result.end(), mDebugRequiredInstanceExtensions.begin(), mDebugRequiredInstanceExtensions.end());
#endif

		CheckRequiredInstanceExtensionsIsSatisfied(result);
		return result;
	}

	PhysicalDeviceSpecification GfxVulkanDevice::EnumeratePhysicalDeviceSpecificationFor(VkPhysicalDevice device)
	{
		PhysicalDeviceSpecification result;
		result.handle = device;

		vkGetPhysicalDeviceProperties(device, &result.deviceProperties);
		vkGetPhysicalDeviceMemoryProperties(device, &result.memoryProperties);
		vkGetPhysicalDeviceFeatures(device, &result.features);

		uint32_t physicalDeviceExtCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &physicalDeviceExtCount, nullptr);
		result.deviceExtensions.resize(physicalDeviceExtCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &physicalDeviceExtCount, result.deviceExtensions.data());

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(result.handle, &queueFamilyCount, nullptr);
		result.queueFamilyProperties.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(result.handle, &queueFamilyCount, result.queueFamilyProperties.data());

		uint32_t i = 0;
		for (const auto &queueFamily : result.queueFamilyProperties)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				result.queueFamilyIndices.graphicsFamilyIdx = i;
			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				result.queueFamilyIndices.computeFamilyIdx = i;
			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				result.queueFamilyIndices.transferFamilyIdx = i;

			if (result.queueFamilyIndices.IsComplete())
				break;
			i++;
		}

		return result;
	}

	void GfxVulkanDevice::DumpPhysicalDeviceSpecifications()
	{
		CYNIC_ENGINE_LOG_INFO(TEXT("Physical Devices infos:"));

		for (int32_t i = 0; i < mPhysicalDeviceSpecificationList.size(); ++i)
		{
			const auto &spec = mPhysicalDeviceSpecificationList[i];
			CYNIC_ENGINE_LOG_INFO(TEXT("Physical Device {}: {}, Type: {}, API Version: {}, Driver Version: {}"),
								  i, spec.deviceProperties.deviceName, spec.deviceProperties.deviceType,
								  spec.deviceProperties.apiVersion, spec.deviceProperties.driverVersion);
		}
	}
}
#ifndef VULKAN_HPP
#define VULKAN_HPP

// Std includes
#include <iostream>
#include <string>
#include <vector>

// Lib includes
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

// Local includes
#include <util/types.h>

namespace api {

// Vulkan API
namespace vulkan {
	uint32_t getAppCount();
	void useCallouts(bool callouts);
}
class VulkanApp {
public:
	VulkanApp(VulkanApp&) = delete;
	VulkanApp(std::string appName, std::string engineName);
	~VulkanApp();
private:
	// Methods
	void createInstance();
	void pickPhysicalDevice();

#ifndef NDEBUG
	// Debug ONLY
	void setupDebugMessenger();
	// Utils
	bool checkValidationLayerSupport();
	void checkSupportedExtensions();
#endif

	// Utils
	std::vector<const char*> getRequiredExtensions();

	// Members
	VkInstance instance;
	std::string appName, engineName;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

#ifndef NDEBUG
	// Debug ONLY
	VkDebugUtilsMessengerEXT debugMessenger;
	const char* validationLayers[1] = { "VK_LAYER_KHRONOS_validation" };

	// Static validation methods
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
	static VkResult createDebugUtilsMessengerEXT(
		VkInstance insatnce,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger
	);
	static void destroyDebugUtilsMessengerEXT(
		VkInstance insatnce,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator
	);
#endif

};

}

#endif
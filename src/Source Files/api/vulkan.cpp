#include <api/vulkan.hpp>
using namespace api::vulkan;

namespace api {

namespace vulkan {
    // Utility
    uint32_t vulkanAppCount = 0;
    bool callouts = false;
    uint32_t getVulkanAppCount() { return vulkanAppCount; }
    void useCallouts(bool value) { callouts = value; }
}

// VulkanApp
VulkanApp::VulkanApp(std::string appName, std::string engineName) {
	this->appName = appName;
    this->engineName = engineName;
    if (callouts) std::cout << "VulkanApp: Creating vulkan application \"" << appName << "\" with engine \"" << engineName << "\"\n";

	// Init Vulkan components
    createInstance();

#ifndef NDEBUG
    // Debug ONLY
	setupDebugMessenger();
#endif

    pickPhysicalDevice();

	vulkanAppCount++;
}
VulkanApp::~VulkanApp() {
	// Discard resources

#ifndef NDEBUG
	// Debug ONLY
	destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

    vkDestroyInstance(instance, nullptr);
	vulkanAppCount--;
	if (callouts) std::cout << "VulkanApp: Destructing vulkan application \"" << appName << "\"\n";
	if (vulkanAppCount < 1 && callouts) std::cout << "VulkanApp: No vulkan applications remaining\n";
}

// Private methods
void VulkanApp::createInstance() {

#ifndef NDEBUG
	// Debug ONLY
	if (!checkValidationLayerSupport()) throw std::runtime_error("VulkanApp: Validation layers requested, but not available!");
	else if (callouts) std::cout << "VulkanApp: Validation layers enabled succesfully\n";
#endif

	// App info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = engineName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Create info
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	std::vector<const char*> extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = (uint32_t) extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
	// Debug ONLY
	// Add validation layers to instance create info
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = validationLayers;

	// Add pNext debug messenger for instance debugging
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debugCreateInfo.pfnUserCallback = debugCallback;
	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
#else
	// Release ONLY
	createInfo.enabledLayerCount = 0;
#endif

	// Create instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) throw std::runtime_error("VulkanApp: Failed to create instance!");
    else if (callouts) std::cout << "VulkanApp: Successfully created vulkan instance for vulkan application \"" << appName << "\"\n";

	// Check for supported extensions
#ifndef NDEBUG
	// Debug ONLY
	checkSupportedExtensions();
#endif

}
void VulkanApp::pickPhysicalDevice() {
    uint32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) throw std::runtime_error("Failed to find GPU's with Vulkan support!");
    VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (int32 i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(devices[i])) {
            physicalDevice = devices[i];
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) throw std::runtime_error("Failed to find suitable GPU!");

    // Free unused memory
    free(devices);
}

#ifndef NDEBUG
// Debug ONLY
void VulkanApp::setupDebugMessenger() {
	if (callouts) std::cout << "VulkanApp: Setting up debug messenger...\n";
	// Setup the debug messenger for Vulkan Validation Layers
	// Populate debug messenger create info
	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;

	// Create debug messenger
	if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	throw std::runtime_error("VulkanApp: Failed to set up debug messenger!");
	if (callouts) std::cout << "VulkanApp: Successfully setup debug messenger\n";
}
// Utils
bool VulkanApp::checkValidationLayerSupport() {
	if (callouts) std::cout << "VulkanApp: Checking for validation layer support...\n";
	// Check for available validation layers
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
	for (uint32_t i = 0; i < 1; i++) {
		bool layerFound = false;
		for (uint32_t j = 0; j < layerCount; j++) {
			if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
				layerFound = true;
				break;
			}
		}
		if (!layerFound) return false;
	}

	// Free unused memory
	free(availableLayers);

	return true;
}
void VulkanApp::checkSupportedExtensions() {
	if (callouts) std::cout << "VulkanApp: Checking for supported extensions...\n";
	// Check for supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);
	std::cout << "VulkanApp: Available extensions:\n";
	for (uint32_t i = 0; i < extensionCount; i++) std::cout << "    " << extensions[i].extensionName << "\n";

	// Free unused memory
	free(extensions);
}
#endif

std::vector<const char*> VulkanApp::getRequiredExtensions() {
	if (callouts) std::cout << "VulkanApp: Getting GLFW required extensions...\n";
	// Get extensions required by GLFW
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
	// Debug ONLY
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	return extensions;
}
VulkanApp::QueueFamilyIndices VulkanApp::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    for (int32 i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphicsFamily.set(i);
        if (indices.isComplete()) break;
    }

    // Free unused memory
    free(queueFamilies);

    return indices;
}
bool VulkanApp::isDeviceSuitable(VkPhysicalDevice device) {
    // Get GPU Specs
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    // Set requirements
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool isSuitable = indices.isComplete();
    return isSuitable;
}

#ifndef NDEBUG
// Debug ONLY
// Static validation methods
VKAPI_ATTR VkBool32 VKAPI_CALL VulkanApp::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	// Validation layers debug callback output
	std::cout << "!!! VulkanApp: VALIDATION LAYER: " << pCallbackData->pMessage << "\n";
	return VK_FALSE;
}
VkResult VulkanApp::createDebugUtilsMessengerEXT(
	VkInstance insatnce,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger
) {
	if (callouts) std::cout << "VulkanApp: Creating debug messenger extension...\n";
	// Callback for creating the debug messenger extesnion
    auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(insatnce, "vkCreateDebugUtilsMessengerEXT");
    if (function == nullptr) return VK_ERROR_EXTENSION_NOT_PRESENT;
    return function(insatnce, pCreateInfo, pAllocator, pDebugMessenger);
}
void VulkanApp::destroyDebugUtilsMessengerEXT(
	VkInstance insatnce,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator
) {
	if (callouts) std::cout << "VulkanApp: Destroying debug messenger extension...\n";
	// Callback for destroying the debug messenger extension
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(insatnce, "vkDestroyDebugUtilsMessengerEXT");
    if (function == nullptr) return;
    function(insatnce, debugMessenger, pAllocator);
}
#endif

}
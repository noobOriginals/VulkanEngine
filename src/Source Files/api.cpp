#include <api.hpp>
using namespace api;
using namespace window;
using namespace vulkan;

void def_framebuffer_size_callback(GLFWwindow* window, int width, int height) {}

namespace api {
	namespace window {
		// Utility
		int windowCount = 0;
		bool wcallouts = false;
		int getWindowCount() { return windowCount; }
		void useCallouts(bool value) { wcallouts = value; }
	}
	namespace vulkan {
		// Utility
		int vulkanAppCount = 0;
		bool vkcallouts = false;
		int getVulkanAppCount() { return vulkanAppCount; }
		void useCallouts(bool value) { vkcallouts = value; }
	}
}

// Window
Window::Window(const int width, const int height, const std::string title) {
	this->title = title;
	this->width = width;
	this->height = height;
	fscreen = false;
	if (wcallouts) std::cout << "Window: Creating window \"" << title << "\"\n";

	// Prepare GLFW for window creation
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create window and associate pointer
	address = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (address == nullptr) {
		std::cerr << "Window: Failed to create window " << title << "\n";
		if (windowCount < 1) {
			glfwTerminate();
			std::cerr << "Window: Terminating GLFW" << "\n";
		}
	}
	glfwMakeContextCurrent(address);

	// Set Window parameters
	glfwSetFramebufferSizeCallback(address, def_framebuffer_size_callback);
	glfwSwapInterval(1);
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwShowWindow(address);

	windowCount++;
}
Window::~Window() {
	// Discard resources
	glfwSetWindowShouldClose(address, true);
	glfwDestroyWindow(address);
	windowCount--;
	if (wcallouts) std::cout << "Window: Destructing window \"" << title << "\"\n";
	if (windowCount < 1) {
		glfwTerminate();
		if (wcallouts) std::cout << "Window: No windows remaining" << "\n";
		if (wcallouts) std::cout << "Window: Terminating GLFW" << "\n";
	}
}
// Utility
void Window::close() {
	glfwSetWindowShouldClose(address, GLFW_TRUE);
}
void Window::flush() {
	glfwSwapBuffers(address);
	glfwPollEvents();
}
void Window::setFramebufferSizeCallback(void(*func)(GLFWwindow* window, int width, int height)) {
	glfwSetFramebufferSizeCallback(address, func);
}
void Window::setCursorPosCallback(void(*func)(GLFWwindow* window, double xpos, double ypos)) {
	glfwSetCursorPosCallback(address, func);
}
GLFWwindow* Window::getAddress() { return address; }
bool Window::shouldClose() {
	return glfwWindowShouldClose(address);
}
void Window::fullscreen() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(address, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	fscreen = true;
}
void Window::windowed() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowMonitor(address, nullptr, (int)(mode->width / 2.0f - width / 2.0f), (int)(mode->height / 2.0f - height / 2.0f), width, height, GLFW_DONT_CARE);
	fscreen = false;
}
bool Window::isFullscreen() {
	return fscreen;
}
bool Window::isResizable() {
    return resizable;
}
bool Window::keyPressed(short key) {
	return (glfwGetKey(address, key) == GLFW_PRESS);
}
bool Window::keyTyped(short key, bool& schedule) {
	if (keyPressed(key)) {
		schedule = true;
	}
	if (schedule && !keyPressed(key)) {
		schedule = false;
		return true;
	}
	else {
		return false;
	}
}
bool Window::mousePressed(short button) {
	return (glfwGetMouseButton(address, button) == GLFW_PRESS);
}
void Window::makeCurrentContext() {
	glfwMakeContextCurrent(address);
}
void Window::releaseMouse() {
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Window::captureMouse() {
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::show() {
	glfwShowWindow(address);
}
void Window::hide() {
	glfwHideWindow(address);
}
void Window::setWindowResizable(bool resizable) {
    glfwSetWindowAttrib(address, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    this->resizable = resizable;
}
void Window::getSize(int* width, int* height) {
	glfwGetWindowSize(address, width, height);
}
void Window::enableVSync() {
	glfwSwapInterval(1);
}
void Window::disableVSync() {
	glfwSwapInterval(0);
}


// VulkanApp
VulkanApp::VulkanApp(std::string appName, std::string engineName) {
    this->appName = appName;
    this->engineName = engineName;
    if (vkcallouts) std::cout << "VulkanApp: Creating vulkan application \"" << appName << "\" with engine \"" << engineName << "\"\n";
	// Init Vulkan components
    createInstance();
#ifndef NDEBUG
	setupDebugMessenger();
#endif

	vulkanAppCount++;
}
VulkanApp::~VulkanApp() {
	// Discard resources
#ifndef NDEBUG
	destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif
    vkDestroyInstance(instance, nullptr);
	vulkanAppCount--;
	if (vkcallouts) std::cout << "VulkanApp: Destructing vulkan application \"" << appName << "\"\n";
	if (vulkanAppCount < 1 && vkcallouts) std::cout << "VulkanApp: No vulkan applications remaining\n";
}

// Private methods
void VulkanApp::createInstance() {
#ifndef NDEBUG
	if (!checkValidationLayerSupport()) throw std::runtime_error("VulkanApp: Validation layers requested, but not available!");
	else if (vkcallouts) std::cout << "VulkanApp: Validation layers enabled succesfully\n";
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
	createInfo.enabledLayerCount = 0;
#endif

	// Create instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) throw std::runtime_error("VulkanApp: Failed to create instance!");
    else if (vkcallouts) std::cout << "VulkanApp: Successfully created vulkan instance for vulkan application \"" << appName << "\"\n";

	// Check for supported extensions
#ifndef NDEBUG
	checkSupportedExtensions();
#endif
}
#ifndef NDEBUG
void VulkanApp::setupDebugMessenger() {
	if (vkcallouts) std::cout << "VulkanApp: Setting up debug messenger...\n";
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
	if (vkcallouts) std::cout << "VulkanApp: Successfully setup debug messenger\n";
}
// Utils
bool VulkanApp::checkValidationLayerSupport() {
	if (vkcallouts) std::cout << "VulkanApp: Checking for validation layer support...\n";
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
	if (vkcallouts) std::cout << "VulkanApp: Checking for supported extensions...\n";
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
	if (vkcallouts) std::cout << "VulkanApp: Getting GLFW required extensions...\n";
	// Get extensions required by GLFW
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
#ifndef NDEBUG
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	return extensions;
}

#ifndef NDEBUG
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
	if (vkcallouts) std::cout << "VulkanApp: Creating debug messenger extension...\n";
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
	if (vkcallouts) std::cout << "VulkanApp: Destroying debug messenger extension...\n";
	// Callback for destroying the debug messenger extension
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(insatnce, "vkDestroyDebugUtilsMessengerEXT");
    if (function == nullptr) return;
    function(insatnce, debugMessenger, pAllocator);
}
#endif

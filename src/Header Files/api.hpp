#ifndef API_H
#define API_H

// Std includes
#include <iostream>
#include <string>
#include <vector>

// Lib includes
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

// Local Includes

namespace api {

// Window api
namespace window {
	uint32_t getWindowCount();
	void useCallouts(bool callouts);
}
class Window {
public:
	Window() { address = 0; width = 0; height = 0; title = ""; fscreen = false; }
	Window(Window& w) = delete;
	Window(const uint32_t width, const uint32_t height, const std::string title);
	~Window();
	// Utility
	void close();
	void flush();
	void setFramebufferSizeCallback(void(*func)(GLFWwindow* window, int32_t width, int32_t height));
	void setCursorPosCallback(void(*func)(GLFWwindow* window, double_t xpos, double_t ypos));
	GLFWwindow* getAddress();
	void fullscreen();
	void windowed();
	bool isFullscreen();
	bool isResizable();
	bool shouldClose();
	bool keyPressed(int16_t key);
	bool keyTyped(int16_t key, bool& schedule);
	bool mousePressed(int16_t button);
	void makeCurrentContext();
	void releaseMouse();
	void captureMouse();
	void show();
	void hide();
	void setWindowResizable(bool resizeable);
	void getSize(int32_t* width, int32_t* height);
	void enableVSync();
	void disableVSync();
private:
	std::string title;
	GLFWwindow* address;
	int width, height;
	bool fscreen, resizable;
};

// Vulkan api
namespace vulkan {
	uint32_t getAppCount();
	void useCallouts(bool callouts);
}
class VulkanApp {
public:
	VulkanApp(std::string appName, std::string engineName);
	VulkanApp(VulkanApp&) = delete;
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
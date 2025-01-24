#include <api.hpp>
using namespace api;
using namespace window;

void def_framebuffer_size_callback(GLFWwindow* window, int width, int height) {}

namespace api {
	namespace window {
		// Utility
		int windowCount = 0;
		bool callouts = false;
		int getWindowCount() { return windowCount; }
		void useCallouts(bool value) { callouts = value; }
	}
	namespace vulkan {
		// Utility
		// int vulkanAppCount = 0;
		bool callouts = false;
		// int getWindowCount() { return windowCount; }
		void useCallouts(bool value) { callouts = value; }
	}
}

// Window
Window::Window(const int width, const int height, const std::string title) {
	this->title = title;
	this->width = width;
	this->height = height;
	fscreen = false;
	if (callouts) {
		std::cout << "Window: Creating window \"" << title << "\"\n";
	}

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
	// glfwSetFramebufferSizeCallback(address, def_framebuffer_size_callback);
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
	if (callouts) {
		std::cout << "Window: Destructing window \"" << title << "\"\n";
	}
	if (windowCount < 1) {
		glfwTerminate();
		std::cout << "Window: Terminating GLFW" << "\n";
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
	glfwSetWindowMonitor(address, nullptr, mode->width / 2.0f - width / 2.0f, mode->height / 2.0f - height / 2.0f, width, height, GLFW_DONT_CARE);
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
    if (callouts) {
		std::cout << "VulkanApp: Creating vulkan application \"" << appName << "\" with engine \"" << engineName << "\"\n";
    }

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
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    // Create instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "VulkanApp: Failed to create instance!\n";
    }
    else if (callouts) {
        std::cout << "VulkanApp: Successfully created vulkan instance for vulkan application \"" << appName << "\"\n";
    }
}
VulkanApp::~VulkanApp() {
    vkDestroyInstance(instance, nullptr);
	if (callouts) {
		std::cout << "VulkanApp: Destructing vulkan application \"" << appName << "\"\n";
	}
}
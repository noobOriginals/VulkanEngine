#ifndef API_H
#define API_H

// Std includes
#include <iostream>
#include <string>
#include <vector>

// Lib includes
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// Local Includes

namespace api {
	// Window api
	namespace window {
		int getWindowCount();
		void useCallouts(bool callouts);
	}
	class Window {
	public:
		Window() { address = 0; width = 0; height = 0; title = ""; fscreen = false; }
		Window(Window& w) = delete;
		Window(const int width, const int height, const std::string title);
		~Window();
		// Utility
		void close();
		void flush();
		void setFramebufferSizeCallback(void(*func)(GLFWwindow* window, int width, int height));
		void setCursorPosCallback(void(*func)(GLFWwindow* window, double xpos, double ypos));
		GLFWwindow* getAddress();
		void fullscreen();
		void windowed();
		bool isFullscreen();
        bool isResizable();
		bool shouldClose();
		bool keyPressed(short key);
		bool keyTyped(short key, bool& schedule);
		bool mousePressed(short button);
		void makeCurrentContext();
		void releaseMouse();
		void captureMouse();
		void show();
		void hide();
        void setWindowResizable(bool resizeable);
		void getSize(int* width, int* height);
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
		// int getAppCount();
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
		void setupDebugMessenger();

		// Utils
		bool checkValidationLayerSupport();
		void checkSupportedExtensions();
		std::vector<const char*> getRequiredExtensions();

        // Members
        VkInstance instance;
        std::string appName, engineName;
		VkDebugUtilsMessengerEXT debugMessenger;
		const char* validationLayers[1] = {
			"VK_LAYER_KHRONOS_validation"
		};

		// Static validation callbacks
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		) {
			std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n";
			return VK_FALSE;
		}
    };
}

#endif
#ifndef API_H
#define API_H

// Std includes
#include <iostream>
#include <string>

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
        // Members
        VkInstance instance;
        std::string appName, engineName;
    };
}

#endif
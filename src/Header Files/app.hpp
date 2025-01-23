#ifndef APP_H
#define APP_H

// Std includes
#include <iostream>

// Lib includes
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

class App {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }
private:
    // Methods
    void initWindow() {
        glfwInit();
	    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Engine", nullptr, nullptr);
    }
    void initVulkan() {
        createInstance();
    }
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }
    void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void createInstance() {
        // App info
        VkApplicationInfo appInfo;
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Create info
        VkInstanceCreateInfo createInfo;
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
            std::cerr << "Failed to create instance!\n";
        }
    }
    
    // Members
    const uint32_t WIDTH = 800, HEIGHT = 600;
    GLFWwindow* window;
    VkInstance instance;
};

#endif
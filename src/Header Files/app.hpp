#ifndef APP_H
#define APP_H

// Std includes
#include <iostream>
#include <vector>

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
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

        // App info
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Engine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
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
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = validationLayers;
        }
        else {
            createInfo.enabledLayerCount = 0;
        }

        // Create instance
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance!");
        }

        // Check for extension support
        if (enableValidationLayers) {
            checkExtensionSupport();
        }
    }
    void checkExtensionSupport() {
        // Check for extension support
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);
        std::cout << "Available extensions:\n";
        for (int i = 0; i < extensionCount; i++) {
            std::cout << extensions[i].extensionName << "\n";
        }
        std::cout << "\n";
    }
    bool checkValidationLayerSupport() {
        // Check for available validation layers
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
        for (int i = 0; i < 1; i++) {
            bool layerFound = false;
            for (int j = 0; j < layerCount; j++) {
                if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }
    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    }
    
    // Members
    const uint32_t WIDTH = 800, HEIGHT = 600;
    GLFWwindow* window;
    VkInstance instance;
    const char* validationLayers[1] = {
        "VK_LAYER_KHRONOS_validation"
    };
#ifndef NDEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif
};

#endif
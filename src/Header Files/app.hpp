#ifndef APP_H
#define APP_H

// Std includes
#include <iostream>
#include <vector>

// Lib includes
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

namespace app {

class App {
public:
    void run();
private:
    // Data types
    struct QueueFamily {
        uint32_t index;
        bool hasValue = false;
        void set(uint32_t i) {
            index = i;
            hasValue = true;
        }
    };
    struct QueueFamilyIndices {
        QueueFamily graphicsFamily;

        bool isComplete() {
            return graphicsFamily.hasValue;
        }
    };

    // Methods
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createInstance();

#ifndef NDEBUG
    // Debug ONLY
    void checkSupportedExtensions();
    bool checkValidationLayerSupport();
#endif

    std::vector<const char*> getRequiredExtensions();
    void pickPhysicalDevice();
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

#ifndef NDEBUG
    // Debug ONLY
    void setupDebugMessenger();
#endif

    // Check for requirements of GPU specs
    bool isDeviceSuitable(VkPhysicalDevice device);
    
    // Members
    const uint32_t WIDTH = 800, HEIGHT = 600;
    GLFWwindow* window;
    VkInstance instance;
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
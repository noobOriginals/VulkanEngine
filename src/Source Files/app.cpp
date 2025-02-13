#include <app.hpp>
using namespace std;

namespace app {

// Public
void App::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

// Private
void App::initWindow() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create the window
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Engine", nullptr, nullptr);
}
void App::initVulkan() {
    createInstance();

#ifndef NDEBUG
    //Debug ONLY
    setupDebugMessenger();
#endif

    pickPhysicalDevice();
}
void App::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}
void App::cleanup() {

#ifndef NDEBUG
    // Debug ONLY
    destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::createInstance() {

#ifndef NDEBUG
    // Debug ONLY
    if (!checkValidationLayerSupport()) throw runtime_error("Validation layers requested, but not available!");
#endif

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
    vector<const char*> extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = (uint32) extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
    // Debug ONLY
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;

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
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) throw runtime_error("Failed to create instance!");
    
#ifndef NDEBUG
    // Check for extension support
    // Debug ONLY
    checkSupportedExtensions();
#endif

}

#ifndef NDEBUG
// Debug ONLY
void App::checkSupportedExtensions() {
    // Check for supported extensions
    uint32 extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions);
    cout << "\nAvailable extensions:\n";
    for (uint32 i = 0; i < extensionCount; i++) {
        cout << extensions[i].extensionName << "\n";
    }
    cout << "\n";

    // Free unused memory
    free(extensions);
}
bool App::checkValidationLayerSupport() {
    // Check for available validation layers
    uint32 layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    VkLayerProperties* availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);
    for (uint32 i = 0; i < 1; i++) {
        bool layerFound = false;
        for (uint32 j = 0; j < layerCount; j++) {
            if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }

    // Free unused memory
    free(availableLayers);

    return true;
}
void App::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    
    if (createDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    throw std::runtime_error("Failed to set up debug messenger!");
}
#endif

vector<const char*> App::getRequiredExtensions() {
    // Get extensions required by GLFW
    uint32 glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
// Debug ONLY
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    return extensions;
}

void App::pickPhysicalDevice() {
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
// Check for requirements of GPU specs
bool App::isDeviceSuitable(VkPhysicalDevice device) {
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

App::QueueFamilyIndices App::findQueueFamilies(VkPhysicalDevice device) {
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

#ifndef NDEBUG
    // Debug ONLY
    // Static validation methods
    VKAPI_ATTR VkBool32 VKAPI_CALL App::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    ) {
        std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n";
        return VK_FALSE;
    }
    VkResult App::createDebugUtilsMessengerEXT(
        VkInstance insatnce,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    ) {
        auto function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(insatnce, "vkCreateDebugUtilsMessengerEXT");
        if (function == nullptr) return VK_ERROR_EXTENSION_NOT_PRESENT;
        return function(insatnce, pCreateInfo, pAllocator, pDebugMessenger);
    }
    void App::destroyDebugUtilsMessengerEXT(
        VkInstance insatnce,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator
    ) {
        auto function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(insatnce, "vkDestroyDebugUtilsMessengerEXT");
        if (function == nullptr) return;
        function(insatnce, debugMessenger, pAllocator);
    }
#endif

}
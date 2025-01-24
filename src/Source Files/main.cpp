// Std includes

// Lib includes

// Local Includes
#include <api.hpp>

int main() {
    api::window::useCallouts(true);
    api::vulkan::useCallouts(true);
    api::Window window(800, 600, "Vulkan Engine");
    window.setWindowResizable(false);
    api::VulkanApp vulkanApp("Vulkan Engine", "No Engine");
    while (!window.shouldClose()) {
        window.flush();
    }
	return 0;
}
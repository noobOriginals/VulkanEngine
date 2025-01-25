// Std includes

// Lib includes

// Local Includes
#include <api.hpp>
// #include <app.hpp>

void test() {
    api::window::useCallouts(true);
    api::vulkan::useCallouts(true);
    api::Window window(800, 600, "Vulkan Engine");
    window.setWindowResizable(false);
    api::VulkanApp vulkanApp("Vulkan Engine", "No engine");
    while (!window.shouldClose()) {
        window.flush();
    }
}

// void test() {
//     App app;
//     app.run();
// }

int main() {
    test();
	return 0;
}
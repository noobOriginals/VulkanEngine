// Std includes

// Lib includes

// Local Includes

// Test API
// #include <api.hpp>
// using namespace api;

// Test App
#include <app.hpp>
using namespace app;

// void test() {
// #ifndef NDEBUG
//     api::window::useCallouts(true);
//     api::vulkan::useCallouts(true);
// #endif
//     api::Window window(800, 600, "Vulkan Engine");
//     window.setWindowResizable(false);
//     api::VulkanApp vulkanApp("Vulkan Engine", "No engine");
//     while (!window.shouldClose()) {
//         window.flush();
//     }
// }

void test() {
    App app;
    app.run();
}

int main() {
    test();
	return 0;
}
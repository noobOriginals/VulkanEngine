#ifndef APP_H
#define APP_H

// Std includes
#include <iostream>

// Lib includes
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
    
    // Members
    const int WIDTH = 800, HEIGHT = 600;
    GLFWwindow* window;
};

#endif
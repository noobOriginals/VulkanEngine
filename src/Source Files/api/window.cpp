#include <api/window.hpp>
using namespace api::window;

void def_framebuffer_size_callback(GLFWwindow* window, int32 width, int32 height) {}

namespace api {

namespace window {
	// Utility
	uint32 windowCount = 0;
	bool callouts = false, glfwTerminated = true;
	std::string focusedWindow;
	uint32 getWindowCount() { return windowCount; }
	void useCallouts(bool value) { callouts = value; }
	std::string getFocusedWindow() { return focusedWindow; }
}

Window::Window(const uint32 width, const uint32 height, const std::string title) {
	this->title = title;
	this->width = width;
	this->height = height;
	fscreen = false;
	resizable = true;
	closed = false;
	if (callouts) std::cout << "Window: Creating window \"" << title << "\"\n";

	// Prepare GLFW for window creation
	if (glfwTerminated) {
		glfwInit();
		glfwTerminated = false;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create window and associate pointer
	address = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (address == nullptr) throw std::runtime_error("Window: Failed to create window " + title + "\n");
	glfwMakeContextCurrent(address);

	// Set Window parameters
	glfwSetFramebufferSizeCallback(address, def_framebuffer_size_callback);
	glfwSwapInterval(1);
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwShowWindow(address);

	windowCount++;
}
Window::~Window() {
	// Discard resources
	if (!closed) {
		glfwSetWindowShouldClose(address, true);
		glfwDestroyWindow(address);
		windowCount--;
		if (callouts) std::cout << "Window: Destructing window \"" << title << "\"\n";
	}
	if (windowCount < 1 && !glfwTerminated) {
		glfwTerminate();
		if (callouts) std::cout << "Window: No windows remaining" << "\n";
		if (callouts) std::cout << "Window: Terminating GLFW" << "\n";
		glfwTerminated = true;
	}
	closed = true;
}
// Utility
void Window::close() {
	// Close and discard resources
	if (!closed) {
		glfwSetWindowShouldClose(address, true);
		glfwDestroyWindow(address);
		windowCount--;
		if (callouts) std::cout << "Window: Closing window \"" << title << "\"\n";
	}
	if (windowCount < 1 && !glfwTerminated) {
		glfwTerminate();
		if (callouts) std::cout << "Window: No windows remaining" << "\n";
		if (callouts) std::cout << "Window: Terminating GLFW" << "\n";
		glfwTerminated = true;
	}
	closed = true;
}
void Window::flush() {
	glfwSwapBuffers(address);
	glfwPollEvents();
}
void Window::setTitle(std::string title) {
	glfwSetWindowTitle(address, title.c_str());
	this->title = title;
}
void Window::setFramebufferSizeCallback(void(*func)(GLFWwindow* window, int32 width, int32 height)) {
	glfwSetFramebufferSizeCallback(address, func);
}
void Window::resetFramebufferSizeCallback() {
	glfwSetFramebufferSizeCallback(address, def_framebuffer_size_callback);
}
void Window::setCursorPosCallback(void(*func)(GLFWwindow* window, float64 xpos, float64 ypos)) {
	glfwSetCursorPosCallback(address, func);
}
GLFWwindow* Window::getAddress() { return address; }
bool Window::shouldClose() {
	return glfwWindowShouldClose(address);
}
void Window::fullscreen() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(address, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	fscreen = true;
}
void Window::windowed() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowMonitor(address, nullptr, (int32)(mode->width / 2.0f - width / 2.0f), (int32)(mode->height / 2.0f - height / 2.0f), width, height, GLFW_DONT_CARE);
	fscreen = false;
}
bool Window::isFullscreen() {
	return fscreen;
}
bool Window::isResizable() {
	return resizable;
}
bool Window::keyPressed(int16 key) {
	return (glfwGetKey(address, key) == GLFW_PRESS);
}
bool Window::keyTyped(int16 key, bool& schedule) {
	if (keyPressed(key)) {
		schedule = true;
	}
	if (schedule && !keyPressed(key)) {
		schedule = false;
		return true;
	}
	else {
		return false;
	}
}
bool Window::mousePressed(int16 button) {
	return (glfwGetMouseButton(address, button) == GLFW_PRESS);
}
void Window::focus() {
	glfwMakeContextCurrent(address);
	focusedWindow = title;
}
void Window::releaseMouse() {
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Window::captureMouse() {
	glfwSetInputMode(address, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::show() {
	glfwShowWindow(address);
}
void Window::hide() {
	glfwHideWindow(address);
}
void Window::setWindowResizable(bool resizable) {
	glfwSetWindowAttrib(address, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    this->resizable = resizable;
}
void Window::getSize(int32* width, int32* height) {
	glfwGetWindowSize(address, width, height);
}
void Window::enableVSync() {
	glfwSwapInterval(1);
}
void Window::disableVSync() {
	glfwSwapInterval(0);
}

}
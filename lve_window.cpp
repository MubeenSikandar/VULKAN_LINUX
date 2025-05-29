#include "lve_window.hpp"
#include <stdexcept>

namespace lve {
    LveWindow::LveWindow(int width, int height, std::string windowName)
        : width(width), height(height), windowName(windowName){
        initWindow();
    }

     LveWindow::~LveWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void LveWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if (!window) {
            throw std::runtime_error("Failed to create GLFW window");
        }

    }

     void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
         if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
             throw std::runtime_error("failed to create window surface!");
         }
     }

}

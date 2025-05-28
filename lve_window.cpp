#include "lve_window.hpp"

namespace lve {
    LveWindow::LveWindow(int width, int height, std::string name)
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
      
    }
   
}
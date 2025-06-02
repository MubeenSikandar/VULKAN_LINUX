#pragma once

#include "lve_device.hpp"
#include "lve_gameobject.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"
#include "vulkan/vulkan_core.h"

// std
#include <vector>

namespace lve {
class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp&) = delete;
    FirstApp& operator=(const FirstApp&) = delete;

    void run();

  private:
    void loadGameObjects();
    void renderGameObjects(VkCommandBuffer commandBuffer);

    LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};
    std::vector<LveGameObject> gameObjects;
};
} // namespace lve

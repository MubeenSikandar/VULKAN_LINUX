#include "../include/first_app.hpp"
#include "../include/simple_render_system.hpp"

// std
#include <cassert>
#include <cstddef>
#include <glm/gtc/constants.hpp>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getRenderPass()};
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                         {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                         {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveGameObject::createGameObject();
  triangle.model = lveModel;
  triangle.color = {0.5f, 0.0f, 0.5f};
  triangle.transform2d.translation.x = 0.2f;
  triangle.transform2d.scale = {2.0f, 0.5f};
  triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

} // namespace lve

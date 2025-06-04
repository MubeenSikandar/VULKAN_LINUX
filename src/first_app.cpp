#include "../include/first_app.hpp"
#include "../include/keyboard_movement_controller.hpp"
#include "../include/lve_camera.hpp"
#include "../include/lve_gameobject.hpp"
#include "../include/simple_render_system.hpp"

// std
#include <cassert>
#include <chrono>
#include <cstddef>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getRenderPass()};
  LveCamera camera{};
  camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f),
                       glm::vec3(0.0f, 0.0f, 2.5f));

  auto viewerObject = LveGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    float aspect = lveRenderer.getAspectRatio();

    auto time = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(time -
                                                                   currentTime)
            .count();
    currentTime = time;

    cameraController.moveInPlaneXZ(lveWindow.getWindow(), frameTime,
                                   viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.translation);

    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.0f);
    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

std::unique_ptr<LveModel> createFaceModel(LveDevice &device, glm::vec3 offset) {
  using Vertex = LveModel::Vertex;
  std::vector<Vertex> vertices;

  auto addCube = [&](glm::vec3 center, glm::vec3 size, glm::vec3 color) {
    float x = size.x / 2, y = size.y / 2, z = size.z / 2;
    glm::vec3 corners[8] = {
        center + glm::vec3(-x, -y, -z), center + glm::vec3(x, -y, -z),
        center + glm::vec3(x, y, -z),   center + glm::vec3(-x, y, -z),
        center + glm::vec3(-x, -y, z),  center + glm::vec3(x, -y, z),
        center + glm::vec3(x, y, z),    center + glm::vec3(-x, y, z),
    };
    uint32_t idx[] = {
        0, 1, 2, 0, 2, 3, // back
        4, 5, 6, 4, 6, 7, // front
        0, 1, 5, 0, 5, 4, // bottom
        2, 3, 7, 2, 7, 6, // top
        1, 2, 6, 1, 6, 5, // right
        3, 0, 4, 3, 4, 7  // left
    };
    for (int i = 0; i < 36; i++) {
      vertices.push_back({corners[idx[i]], color});
    }
  };

  auto addPyramid = [&](glm::vec3 center, float base, float height,
                        glm::vec3 color) {
    glm::vec3 top = center + glm::vec3(0, height / 2.0f, 0);
    glm::vec3 base1 = center + glm::vec3(-base / 2, -height / 2, -base / 2);
    glm::vec3 base2 = center + glm::vec3(base / 2, -height / 2, -base / 2);
    glm::vec3 base3 = center + glm::vec3(base / 2, -height / 2, base / 2);
    glm::vec3 base4 = center + glm::vec3(-base / 2, -height / 2, base / 2);

    vertices.insert(vertices.end(), {
                                        {top, color},
                                        {base1, color},
                                        {base2, color},
                                        {top, color},
                                        {base2, color},
                                        {base3, color},
                                        {top, color},
                                        {base3, color},
                                        {base4, color},
                                        {top, color},
                                        {base4, color},
                                        {base1, color},
                                        {base1, color},
                                        {base2, color},
                                        {base3, color},
                                        {base1, color},
                                        {base3, color},
                                        {base4, color},
                                    });
  };

  // -----------------------------
  // 🧑 Head (main face block)
  // -----------------------------
  addCube(offset + glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.3f, 1.f),
          {1.0f, 0.85f, 0.75f});

  // -----------------------------
  // 👀 Eyes (white cubes + pupils)
  // -----------------------------
  glm::vec3 eyeColor = {1.0f, 1.0f, 1.0f};
  glm::vec3 pupilColor = {0.1f, 0.1f, 0.1f};

  addCube(offset + glm::vec3(-0.25f, 0.2f, 0.51f),
          glm::vec3(0.2f, 0.15f, 0.05f), eyeColor);
  addCube(offset + glm::vec3(0.25f, 0.2f, 0.51f), glm::vec3(0.2f, 0.15f, 0.05f),
          eyeColor);

  addCube(offset + glm::vec3(-0.25f, 0.2f, 0.55f),
          glm::vec3(0.08f, 0.08f, 0.02f), pupilColor);
  addCube(offset + glm::vec3(0.25f, 0.2f, 0.55f),
          glm::vec3(0.08f, 0.08f, 0.02f), pupilColor);

  // -----------------------------
  // 🪵 Eyebrows
  // -----------------------------
  glm::vec3 browColor = {0.2f, 0.1f, 0.05f};
  addCube(offset + glm::vec3(-0.25f, 0.35f, 0.51f),
          glm::vec3(0.3f, 0.05f, 0.02f), browColor);
  addCube(offset + glm::vec3(0.25f, 0.35f, 0.51f),
          glm::vec3(0.3f, 0.05f, 0.02f), browColor);

  // -----------------------------
  // 👃 Nose (center pyramid)
  // -----------------------------
  addPyramid(offset + glm::vec3(0.f, 0.05f, 0.52f), 0.15f, 0.2f,
             {0.95f, 0.75f, 0.6f});

  // -----------------------------
  // 👄 Mouth (slight curve: 3 segments)
  // -----------------------------
  glm::vec3 mouthColor = {0.7f, 0.1f, 0.1f};
  addCube(offset + glm::vec3(-0.2f, -0.4f, 0.51f),
          glm::vec3(0.15f, 0.07f, 0.02f), mouthColor);
  addCube(offset + glm::vec3(0.0f, -0.45f, 0.51f),
          glm::vec3(0.15f, 0.07f, 0.02f), mouthColor);
  addCube(offset + glm::vec3(0.2f, -0.4f, 0.51f),
          glm::vec3(0.15f, 0.07f, 0.02f), mouthColor);

  glm::mat4 flipY = glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1));
  glm::mat4 flipX = glm::scale(glm::mat4(1.0f), glm::vec3(-1, 1, 1));

  for (auto &v : vertices) {

    glm::vec4 p = glm::vec4(v.position + offset, 1.0f);
    v.position = glm::vec3(flipY * p);
    v.position = glm::vec3(flipX * p);
  }

  return std::make_unique<LveModel>(device, vertices);
}

std::unique_ptr<LveModel> createCubeModel(LveDevice &device, glm::vec3 offset) {
  std::vector<LveModel::Vertex> vertices{

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

  };
  for (auto &v : vertices) {
    v.position += offset;
  }
  return std::make_unique<LveModel>(device, vertices);
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<LveModel> lveModel =
      createFaceModel(lveDevice, {0.0f, 0.0f, 0.0f});

  auto cube = LveGameObject::createGameObject();
  cube.model = lveModel;
  cube.transform.translation = {0.0f, 0.0f, 2.5f};
  cube.transform.scale = {0.5f, 0.5f, 0.5f};
  gameObjects.push_back(std::move(cube));
}

} // namespace lve

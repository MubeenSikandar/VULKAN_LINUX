#pragma once

#include "lve_camera.hpp"
#include "lve_device.hpp"
#include "lve_gameobject.hpp"
#include "lve_pipeline.hpp"
#include "vulkan/vulkan_core.h"

// std
#include <memory>
#include <vector>

namespace lve {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<LveGameObject> &gameObjects,
                         const LveCamera &camera);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;
  VkPipelineLayout pipelineLayout;
  std::unique_ptr<LvePipeline> lvePipeline;
};
} // namespace lve

#pragma once

#include "lve_device.hpp"
#include "lve_swapchain.hpp"
#include "lve_window.hpp"

#include "vulkan/vulkan_core.h"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace lve {
class LveRenderer {
public:
  LveRenderer(LveWindow &lveWindow, LveDevice &lveDevice);
  ~LveRenderer();

  LveRenderer(const LveRenderer &) = delete;
  LveRenderer &operator=(const LveRenderer &) = delete;

  VkRenderPass getRenderPass() const { return lveSwapChain->getRenderPass(); }
  float getAspectRatio() const { return lveSwapChain->extentAspectRatio(); }
  bool isFrameInProgress() const { return isFrameStarted; }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted &&
           "Cannot get command buffer when frame is not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getCurrentFrameIndex() const {
    assert(isFrameStarted &&
           "Cannot get frame index when frame is not in progress");

    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  LveWindow &lveWindow;
  LveDevice &lveDevice;
  std::unique_ptr<LveSwapChain> lveSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex{};
  int currentFrameIndex{0};
  bool isFrameStarted{false};
};
} // namespace lve

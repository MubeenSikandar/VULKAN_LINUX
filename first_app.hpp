#pragma once

#include "lve_swapchain.hpp"
#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include <memory>
#include <vulkan/vulkan_core.h>

namespace lve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        std::string TITLE = "First App";

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp&) = delete;
        FirstApp operator=(const FirstApp&) = delete;


        void run();

    private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();


    LveWindow lveWindow{WIDTH, HEIGHT, TITLE};
    LveDevice lveDevice{lveWindow};
    LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    };
}

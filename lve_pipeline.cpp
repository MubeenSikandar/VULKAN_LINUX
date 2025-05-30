#include "lve_pipeline.hpp"
#include "lve_device.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include<cassert>

namespace lve {

    LvePipeline::LvePipeline(LveDevice &device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo &configInfo) : lveDevice(device)
    {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    LvePipeline::~LvePipeline(){
        vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
        vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
        vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
    }

    std::vector<char> LvePipeline::readFile(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!" + filepath);
        }
        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    void LvePipeline::createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo) {
      assert(
          configInfo.pipelineLayout != VK_NULL_HANDLE &&
          "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
      assert(
          configInfo.renderPass != VK_NULL_HANDLE &&
          "Cannot create graphics pipeline: no renderPass provided in configInfo");

      auto vertCode = readFile(vertFilepath);
      auto fragCode = readFile(fragFilepath);

      createShaderModule(vertCode, &vertShaderModule);
      createShaderModule(fragCode, &fragShaderModule);

      VkPipelineShaderStageCreateInfo shaderStages[2];
      shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
      shaderStages[0].module = vertShaderModule;
      shaderStages[0].pName = "main";
      shaderStages[0].flags = 0;
      shaderStages[0].pNext = nullptr;
      shaderStages[0].pSpecializationInfo = nullptr;

      shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      shaderStages[1].module = fragShaderModule;
      shaderStages[1].pName = "main";
      shaderStages[1].flags = 0;
      shaderStages[1].pNext = nullptr;
      shaderStages[1].pSpecializationInfo = nullptr;

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.vertexAttributeDescriptionCount = 0;
      vertexInputInfo.pVertexAttributeDescriptions = nullptr;
      vertexInputInfo.vertexBindingDescriptionCount = 0;
      vertexInputInfo.pVertexBindingDescriptions = nullptr;

      VkPipelineViewportStateCreateInfo viewportStateInfo{};
      viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportStateInfo.viewportCount = 1;
      viewportStateInfo.pViewports = &configInfo.viewport;
      viewportStateInfo.scissorCount = 1;
      viewportStateInfo.pScissors = &configInfo.scissor;


      VkPipelineMultisampleStateCreateInfo multisampling{};
      multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
      multisampling.sampleShadingEnable = VK_FALSE;
      multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;  // must NOT be 0
      multisampling.minSampleShading = 1.0f;  // Optional
      multisampling.pSampleMask = nullptr;
      multisampling.alphaToCoverageEnable = VK_FALSE;
      multisampling.alphaToOneEnable = VK_FALSE;

      VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = shaderStages;
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
      pipelineInfo.pViewportState = &viewportStateInfo;
      pipelineInfo.pRasterizationState = &configInfo.rasterizationStateInfo;
      pipelineInfo.pMultisampleState = &multisampling;
      pipelineInfo.pColorBlendState = &configInfo.colorBlendStateInfo;
      pipelineInfo.pDepthStencilState = &configInfo.depthStencilStateInfo;
      pipelineInfo.pDynamicState = nullptr;

      pipelineInfo.layout = configInfo.pipelineLayout;
      pipelineInfo.renderPass = configInfo.renderPass;
      pipelineInfo.subpass = configInfo.subpass;

      pipelineInfo.basePipelineIndex = -1;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;



        if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }
    }

      void LvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
          VkShaderModuleCreateInfo createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
          createInfo.codeSize = code.size();
          createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
          if (
              vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
          {
              throw std::runtime_error("failed to create shader module!");
          }
      }

     PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
         PipelineConfigInfo pipelineConfigInfo{ };

         pipelineConfigInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
         pipelineConfigInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
         pipelineConfigInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

         pipelineConfigInfo.viewport.x = 0.0f;
         pipelineConfigInfo.viewport.y = 0.0f;
         pipelineConfigInfo.viewport.width = static_cast<float>(width);
         pipelineConfigInfo.viewport.height = static_cast<float>(height);
         pipelineConfigInfo.viewport.minDepth = 0.0f;
         pipelineConfigInfo.viewport.maxDepth = 1.0f;

         pipelineConfigInfo.scissor.offset = {0, 0};
         pipelineConfigInfo.scissor.extent = {width, height};

         pipelineConfigInfo.rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
         pipelineConfigInfo.rasterizationStateInfo.depthClampEnable = VK_FALSE;
         pipelineConfigInfo.rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
         pipelineConfigInfo.rasterizationStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
         pipelineConfigInfo.rasterizationStateInfo.lineWidth = 1.0f;
         pipelineConfigInfo.rasterizationStateInfo.cullMode = VK_CULL_MODE_NONE;
         pipelineConfigInfo.rasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
         pipelineConfigInfo.rasterizationStateInfo.depthBiasEnable = VK_FALSE;
         pipelineConfigInfo.rasterizationStateInfo.depthBiasConstantFactor = 0.0f;
         pipelineConfigInfo.rasterizationStateInfo.depthBiasClamp = 0.0f;
         pipelineConfigInfo.rasterizationStateInfo.depthBiasSlopeFactor = 0.0f;

         pipelineConfigInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
         pipelineConfigInfo.colorBlendAttachment.blendEnable = VK_FALSE;
         pipelineConfigInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
         pipelineConfigInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
         pipelineConfigInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
         pipelineConfigInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         pipelineConfigInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
         pipelineConfigInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

         pipelineConfigInfo.colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineConfigInfo.colorBlendStateInfo.logicOpEnable = VK_FALSE;
         pipelineConfigInfo.colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
         pipelineConfigInfo.colorBlendStateInfo.attachmentCount = 1;
         pipelineConfigInfo.colorBlendStateInfo.pAttachments = &pipelineConfigInfo.colorBlendAttachment;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[0] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[1] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[2] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[3] = 0.0f;

         pipelineConfigInfo.depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
         pipelineConfigInfo.depthStencilStateInfo.depthTestEnable = VK_TRUE;
         pipelineConfigInfo.depthStencilStateInfo.depthWriteEnable = VK_TRUE;
         pipelineConfigInfo.depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
         pipelineConfigInfo.depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
         pipelineConfigInfo.depthStencilStateInfo.minDepthBounds = 0.0f;
         pipelineConfigInfo.depthStencilStateInfo.maxDepthBounds = 1.0f;
         pipelineConfigInfo.depthStencilStateInfo.stencilTestEnable = VK_FALSE;
         pipelineConfigInfo.depthStencilStateInfo.front = {};
         pipelineConfigInfo.depthStencilStateInfo.back = {};

         return pipelineConfigInfo;
     }
}

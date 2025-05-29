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

    void LvePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo &configInfo){

        // assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipeline provided in configInfo");
        // assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no renderPass provided in configInfo");

        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStageInfo[2];
        shaderStageInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageInfo[0].module = vertShaderModule;
        shaderStageInfo[0].pName = "main";
        shaderStageInfo[0].flags = 0;
        shaderStageInfo[0].pNext = nullptr;
        shaderStageInfo[0].pSpecializationInfo = nullptr;

        shaderStageInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageInfo[1].module = fragShaderModule;
        shaderStageInfo[1].pName = "main";
        shaderStageInfo[1].flags = 0;
        shaderStageInfo[1].pNext = nullptr;
        shaderStageInfo[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStageInfo;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &configInfo.viewportStateInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationStateInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilStateInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendStateInfo;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.pMultisampleState = &configInfo.multisampleStateInfo;

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

         pipelineConfigInfo.viewportStateInfo.viewportCount = 1;
         pipelineConfigInfo.viewportStateInfo.pViewports = &pipelineConfigInfo.viewport;
         pipelineConfigInfo.viewportStateInfo.scissorCount = 1;
         pipelineConfigInfo.viewportStateInfo.pScissors = &pipelineConfigInfo.scissor;

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
         pipelineConfigInfo.colorBlendAttachment.blendEnable = VK_FALSE;


         pipelineConfigInfo.colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineConfigInfo.colorBlendStateInfo.logicOpEnable = VK_FALSE;
         pipelineConfigInfo.colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
         pipelineConfigInfo.colorBlendStateInfo.attachmentCount = 1;
         pipelineConfigInfo.colorBlendStateInfo.pAttachments = &pipelineConfigInfo.colorBlendAttachment;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[0] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[1] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[2] = 0.0f;
         pipelineConfigInfo.colorBlendStateInfo.blendConstants[3] = 0.0f;

         return pipelineConfigInfo;
     }
}

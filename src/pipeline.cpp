#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

#include "include/pipeline.hpp"
#include "include/model.hpp"

namespace engine {
	Pipeline::Pipeline(Device &device, const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo) : device{device} {
		createVulkanPipeline(vertexShader, fragmentShader, pipelineConfigInfo);
	}

	Pipeline::~Pipeline() {
		vkDestroyShaderModule(device.device(), vertexShaderModule, nullptr);
		vkDestroyShaderModule(device.device(), fragmentShaderModule, nullptr);

		vkDestroyPipeline(device.device(), vulkanPipeline, nullptr);
	}

	void Pipeline::bind(VkCommandBuffer commandBuffer) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);
	}

	PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
		PipelineConfigInfo pipelineConfigInfo{};

		pipelineConfigInfo.inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineConfigInfo.inputAssemblyStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipelineConfigInfo.inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

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

		pipelineConfigInfo.multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineConfigInfo.multisampleStateInfo.sampleShadingEnable = VK_FALSE;
		pipelineConfigInfo.multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pipelineConfigInfo.multisampleStateInfo.minSampleShading = 1.0f;
		pipelineConfigInfo.multisampleStateInfo.pSampleMask = nullptr;
		pipelineConfigInfo.multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
		pipelineConfigInfo.multisampleStateInfo.alphaToOneEnable = VK_FALSE;

		pipelineConfigInfo.colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		pipelineConfigInfo.colorBlendAttachmentState.blendEnable = VK_FALSE;
		pipelineConfigInfo.colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		pipelineConfigInfo.colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		pipelineConfigInfo.colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		pipelineConfigInfo.colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		pipelineConfigInfo.colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		pipelineConfigInfo.colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

		pipelineConfigInfo.colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipelineConfigInfo.colorBlendStateInfo.logicOpEnable = VK_FALSE;
		pipelineConfigInfo.colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
		pipelineConfigInfo.colorBlendStateInfo.attachmentCount = 1;
		pipelineConfigInfo.colorBlendStateInfo.pAttachments = &pipelineConfigInfo.colorBlendAttachmentState;
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

	std::vector<char> Pipeline::readFile(const std::string &path) {
		std::ifstream file{path, std::ios::ate | std::ios::binary};

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void Pipeline::createVulkanPipeline(const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo) {
		assert(pipelineConfigInfo.pipelineLayout != VK_NULL_HANDLE && "Failed to create a Vulkan pipeline: no pipelineLayout provided in pipelineConfigInfo!");
		assert(pipelineConfigInfo.renderPass != VK_NULL_HANDLE && "Failed to create a Vulkan pipeline: no renderPass provided in pipelineConfigInfo!");

		auto vertexShaderCode = readFile(vertexShader);
		auto fragmentShaderCode = readFile(fragmentShader);

		createShaderModule(vertexShaderCode, &vertexShaderModule);
		createShaderModule(fragmentShaderCode, &fragmentShaderModule);

		VkPipelineShaderStageCreateInfo shaderStageInfo[2];

		shaderStageInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageInfo[0].module = vertexShaderModule;
		shaderStageInfo[0].pName = "main";
		shaderStageInfo[0].flags = 0;
		shaderStageInfo[0].pNext = nullptr;
		shaderStageInfo[0].pSpecializationInfo = nullptr;

		shaderStageInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStageInfo[1].module = fragmentShaderModule;
		shaderStageInfo[1].pName = "main";
		shaderStageInfo[1].flags = 0;
		shaderStageInfo[1].pNext = nullptr;
		shaderStageInfo[1].pSpecializationInfo = nullptr;

		auto inputAttributeDescriptions = Model::Vertex::getInputAttributeDescriptions();
		auto inputBindingDescriptions = Model::Vertex::getInputBindingDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputStateInfo{};

		vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(inputAttributeDescriptions.size());
		vertexInputStateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(inputBindingDescriptions.size());
		vertexInputStateInfo.pVertexAttributeDescriptions = inputAttributeDescriptions.data();
		vertexInputStateInfo.pVertexBindingDescriptions = inputBindingDescriptions.data();

		VkPipelineViewportStateCreateInfo viewportStateInfo{};

		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &pipelineConfigInfo.viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &pipelineConfigInfo.scissor;

		VkGraphicsPipelineCreateInfo pipelineInfo{};

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStageInfo;
		pipelineInfo.pVertexInputState = &vertexInputStateInfo;
		pipelineInfo.pInputAssemblyState = &pipelineConfigInfo.inputAssemblyStateInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &pipelineConfigInfo.rasterizationStateInfo;
		pipelineInfo.pMultisampleState = &pipelineConfigInfo.multisampleStateInfo;
		pipelineInfo.pColorBlendState = &pipelineConfigInfo.colorBlendStateInfo;
		pipelineInfo.pDepthStencilState = &pipelineConfigInfo.depthStencilStateInfo;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = pipelineConfigInfo.pipelineLayout;
		pipelineInfo.renderPass = pipelineConfigInfo.renderPass;
		pipelineInfo.subpass = pipelineConfigInfo.subpass;
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vulkanPipeline) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create a Vulkan pipeline!");
		}
	}

	void Pipeline::createShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule) {
		VkShaderModuleCreateInfo shaderModuleCreateInfo {};

		shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfo.codeSize = shaderCode.size();
		shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

		if (vkCreateShaderModule(device.device(), &shaderModuleCreateInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create a shader module!");
		}
	}
}

#pragma once

#include <string>
#include <vector>

#include "device.hpp"

namespace engine {
	struct PipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportStateInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationStateInfo;
		VkPipelineMultisampleStateCreateInfo multisampleStateInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
		VkPipelineColorBlendStateCreateInfo colorBlendStateInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline {
		public:
			Pipeline(Device &device, const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo);
			~Pipeline();

			Pipeline(const Pipeline &) = delete;
			void operator=(const Pipeline &) = delete;

			static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

		private:
			static std::vector<char> readFile(const std::string &path);

			void createVulkanPipeline(const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo);
			void createShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);

			Device &device;

			VkPipeline vulkanPipeline;
			VkShaderModule vertexShaderModule;
			VkShaderModule fragmentShaderModule;
	};
}

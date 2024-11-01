#pragma once

#include <string>
#include <vector>

#include "device.hpp"

namespace engine {
	struct PipelineConfigInfo {};

	class Pipeline {
		public:
			Pipeline(Device &device, const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo);
			~Pipeline() {}

			Pipeline(const Pipeline &) = delete;
			void operator=(const Pipeline &) = delete;

			static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

		private:
			static std::vector<char> readFile(const std::string &path);

			void createPipeline(const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo);
			void createShaderModule(const std::vector<char> &shaderCode, VkShaderModule *shaderModule);

			Device &device;

			VkPipeline vulkanPipeline;
			VkShaderModule vertexShaderModule;
			VkShaderModule fragmentShaderModule;
	};
}

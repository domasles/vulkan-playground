#include <fstream>
#include <iostream>
#include <stdexcept>

#include "include/pipeline.hpp"

namespace engine {
	Pipeline::Pipeline(Device &device, const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo) : device{device} {
		createPipeline(vertexShader, fragmentShader, pipelineConfigInfo);
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

	PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
		PipelineConfigInfo pipelineConfigInfo{};
		return pipelineConfigInfo;
	}

	void Pipeline::createPipeline(const std::string &vertexShader, const std::string &fragmentShader, const PipelineConfigInfo &pipelineConfigInfo) {
		auto vertexShaderCode = readFile(vertexShader);
		auto fragmentShaderCode = readFile(fragmentShader);

		std::cout << "Vertex shader code size: " << vertexShaderCode.size() << " characters" << "\n";
		std::cout << "Fragment shader code size: " << fragmentShaderCode.size() << " characters" << "\n";
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

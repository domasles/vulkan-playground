#include <stdexcept>

#include "include/app.hpp"

namespace engine {
	App::App() {
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	App::~App() {
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	}

	void App::run() {
		while (!window.shouldClose()) {
			glfwPollEvents();
		}
	}

	void App::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		
		if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create a pipeline layout!");
		}
	}

	void App::createPipeline() {
		auto pipelineConfigInfo = Pipeline::defaultPipelineConfigInfo(swapChain.width(), swapChain.height());

		pipelineConfigInfo.renderPass = swapChain.getRenderPass();
		pipelineConfigInfo.pipelineLayout = pipelineLayout;

		pipeline = std::make_unique<Pipeline>(device, "shaders/default.vert.spv", "shaders/default.frag.spv", pipelineConfigInfo);
	}

	void App::createCommandBuffers() {}
	void App::drawFrame() {}
}

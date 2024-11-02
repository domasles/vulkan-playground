#pragma once

#include <memory>
#include <vector>

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

namespace engine {
	class App {
		public:
			App();
			~App();

			App(const App &) = delete;
			App &operator=(const App &) = delete;

			static constexpr int WIDTH = 1600;
			static constexpr int HEIGHT = 900;

			void run();

		private:
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void drawFrame();

			Window window{WIDTH, HEIGHT, "Hello World!"};
			Device device{window};
			SwapChain swapChain{device, window.getExtent2D()};

			std::unique_ptr<Pipeline> pipeline;

			VkPipelineLayout pipelineLayout;

			std::vector<VkCommandBuffer> commandBuffers;
	};
}

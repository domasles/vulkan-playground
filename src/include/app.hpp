#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"

namespace engine {
	class App {
		public:
			static constexpr int WIDTH = 1600;
			static constexpr int HEIGHT = 900;

			void run();

		private:
			Window window{WIDTH, HEIGHT, "Hello World!"};
			Device device{window};
			Pipeline pipeline{device, "shaders/default.vert.spv", "shaders/default.frag.spv", Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)}; // Paths relative to the executable
	};
}

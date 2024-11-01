#pragma once

#include "window.hpp"
#include "pipeline.hpp"

namespace engine {
	class App {
		public:
			static constexpr int WIDTH = 1600;
			static constexpr int HEIGHT = 900;

			void run();

		private:
			Window window{WIDTH, HEIGHT, "Hello World!"};
			Pipeline pipeline{"shaders/default.vert.spv", "shaders/default.frag.spv"}; // Paths relative to the executable
	};
}

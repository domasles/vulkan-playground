#pragma once

#include "window.hpp"

namespace engine {
	class App {
		public:
			static constexpr int WIDTH = 1600;
			static constexpr int HEIGHT = 900;

			void run();

		private:
			Window window{WIDTH, HEIGHT, "Hello World!"};
	};
}

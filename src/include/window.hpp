#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace engine {
	class Window {
		public:
			Window(int width, int height, std::string title);
			~Window();

			Window(const Window &) = delete;
			Window &operator=(const Window &) = delete;

			bool shouldClose() { return glfwWindowShouldClose(window); }
			void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

			VkExtent2D getExtent2D() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		private:
			void initWindow();

			const int width;
			const int height;

			std::string title;

			GLFWwindow *window;
	};
}

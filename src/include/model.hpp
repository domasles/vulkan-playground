#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <vector>

#include "device.hpp"

namespace engine {
	class Model {
		public:
			struct Vertex {
				glm::vec2 position;
				glm::vec3 color;

				static std::vector<VkVertexInputAttributeDescription> getInputAttributeDescriptions();
				static std::vector<VkVertexInputBindingDescription> getInputBindingDescriptions();
			};

			Model(Device &device, const std::vector<Vertex> &vertices);
			~Model();

			Model(const Model &) = delete;
			Model &operator=(const Model &) = delete;

			void bind(VkCommandBuffer commandBuffer);
			void draw(VkCommandBuffer commandBuffer);

		private:
			void createVertexBuffers(const std::vector<Vertex> &vertices);

			Device &device;

			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;

			uint32_t vertexCount;
	};
}
#include <cassert>
#include <cstring>

#include "include/model.hpp"

namespace engine {
	Model::Model(Device &device, const std::vector<Vertex> &vertices) : device{device} {
		createVertexBuffers(vertices);
	}

	Model::~Model() {
		vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
		vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
	}

	void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());

		assert(vertexCount >= 3 && "Vertex count must be greater or equal to 3!");

		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

		void *data;

		vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(device.device(), vertexBufferMemory);
	}

	std::vector<VkVertexInputAttributeDescription> Model::Vertex::getInputAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions(2);

		inputAttributeDescriptions[0].binding = 0;
		inputAttributeDescriptions[0].location = 0;
		inputAttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		inputAttributeDescriptions[0].offset = offsetof(Vertex, position);

		inputAttributeDescriptions[1].binding = 0;
		inputAttributeDescriptions[1].location = 1;
		inputAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		inputAttributeDescriptions[1].offset = offsetof(Vertex, color);

		return inputAttributeDescriptions;
	}

	std::vector<VkVertexInputBindingDescription> Model::Vertex::getInputBindingDescriptions() {
		std::vector<VkVertexInputBindingDescription> inputBindingDescriptions(1);

		inputBindingDescriptions[0].binding = 0;
		inputBindingDescriptions[0].stride = sizeof(Vertex);
		inputBindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return inputBindingDescriptions;
	}

	void Model::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = {vertexBuffer};
		VkDeviceSize offsets[] = {0};

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Model::draw(VkCommandBuffer commandBuffer) {
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
}
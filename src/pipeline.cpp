#include "include/pipeline.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace engine {
	Pipeline::Pipeline(const std::string& vertexShader, const std::string& fragmentShader) {
		createPipeline(vertexShader, fragmentShader);
	}

	std::vector<char> Pipeline::readFile(const std::string& path) {
		std::ifstream file{path, std::ios::ate | std::ios::binary};

		if(!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void Pipeline::createPipeline(const std::string& vertexShader, const std::string& fragmentShader) {
		auto vertexShaderCode = readFile(vertexShader);
		auto fragmentShaderCode = readFile(fragmentShader);

		std::cout << "Vertex shader code size: " << vertexShaderCode.size() << " characters" << "\n";
		std::cout << "Fragment shader code size: " << fragmentShaderCode.size() << " characters" << "\n";
	}
}

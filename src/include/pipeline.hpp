#pragma once

#include <string>
#include <vector>

namespace engine {
	class Pipeline {
		public:
			Pipeline(const std::string& vertexShader, const std::string& fragmentShader);

		private:
			static std::vector<char> readFile(const std::string& path);

			void createPipeline(const std::string& vertexShader, const std::string& fragmentShader);
	};
}
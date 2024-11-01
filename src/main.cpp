#include <cstdlib>
#include <iostream>
#include <exception>

#include "include/app.hpp"

int main() {
	engine::App app{};

	try {
		app.run();
	} catch (const std::exception &exception) {
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

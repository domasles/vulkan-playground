#!/bin/bash

# URLs required for adding LunarG repository
GPG_KEY_URL="https://packages.lunarg.com/lunarg-signing-key-pub.asc"
REPO_URL="https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list"

# Packages required for binary compilation
PACKAGES=(
	"build-essential"
	"mingw-w64"

	"vulkan-sdk"
	"libglm-dev"
	"libxxf86vm-dev"
	"libxi-dev"

	"unzip"
	"wget"
)

# MinGW libraries path
MINGW_W64_LIB_PATH="/usr/x86_64-w64-mingw32/lib"

# URL required for downloading GLFW windows binaries
GLFW_VERSION="3.4"
GLFW_URL="https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.bin.WIN64.zip"

# URL required for downloading Vulkan windows library
VULKAN_LIB_URL="https://raw.githubusercontent.com/SaschaWillems/Vulkan/master/libs/vulkan/vulkan-1.lib"

# Adding the LunarG repository
add_LunarG_repository() {
	echo "Adding the LunarG signing key..."
	wget -qO /tmp/lunarg-signing-key.asc "$GPG_KEY_URL"

	sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/lunarg.gpg /tmp/lunarg-signing-key.asc
	rm /tmp/lunarg-signing-key.asc

	echo "Adding the LunarG Vulkan repository..."
	sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list "$REPO_URL"
}

# Updating package list
update_package_list() {
	echo "Updating the package list..."
	sudo apt update
}

# Instaling required packages
installl_packages() {
	echo "Installing Vulkan SDK and additional libraries..."
	for package in "${PACKAGES[@]}"; do
		sudo apt install -y "$package"
	done
}

# Installing GLFW windows binaries
install_glfw_windows_binaries() {
	echo "Downloading GLFW..."
	wget -q "$GLFW_URL" -O /tmp/glfw.bin.WIN64.zip

	echo "Extracting GLFW..."
	unzip -q /tmp/glfw.bin.WIN64.zip -d /tmp

	echo "Copying libglfw3.a to $MINGW_W64_LIB_PATH..."
	sudo cp /tmp/glfw-${GLFW_VERSION}.bin.WIN64/lib-mingw-w64/libglfw3.a "$MINGW_W64_LIB_PATH"

	echo "Cleaning up..."
	rm -rf /tmp/glfw.zip /tmp/glfw-${GLFW_VERSION}
}

# Installing Vulkan windows binaries
install_vulkan_windows_binaries() {
	echo "Downloading Vulkan library..."
	wget -q "$VULKAN_LIB_URL" -O /tmp/vulkan-1.lib

	echo "Copying vulkan-1.lib to $MINGW_W64_LIB_PATH..."
	sudo cp /tmp/vulkan-1.lib "$MINGW_W64_LIB_PATH"

	echo "Cleaning up..."
	rm /tmp/vulkan-1.lib
}

# Executing functions
add_LunarG_repository
update_package_list
installl_packages
install_glfw_windows_binaries
install_vulkan_windows_binaries

echo "Installation completed successfully!"

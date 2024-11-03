#!/bin/bash

# Packages required for binary compilation
PACKAGES=(
	"build-essential"
	"mingw-w64"

	"vulkan-sdk"
	"libglfw3-dev"
	"libglm-dev"
	"libxxf86vm-dev"
	"libxi-dev"

	"unzip"
)

# URLs required for adding LunarG repository
GPG_KEY_URL="https://packages.lunarg.com/lunarg-signing-key-pub.asc"
REPO_URL="https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list"

# Vulkan key and repository list extracts
LUNARG_GPG=/etc/apt/trusted.gpg.d/lunarg.gpg
LUNARG_REPOSITORY_LIST=/etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list

# MinGW directories
MINGW_W64_LIB_DIR="/usr/x86_64-w64-mingw32/lib"
MINGW_W64_INCLUDE_DIR="/usr/x86_64-w64-mingw32/include"

# URL required for downloading GLFW windows binaries
GLFW_VERSION="3.4"
GLFW_URL="https://github.com/glfw/glfw/releases/download/$GLFW_VERSION/glfw-$GLFW_VERSION.bin.WIN64.zip"

# Temporary directories and extracts
TMP_DIR=/tmp

LUNARG_TMP_KEY=$TMP_DIR/lunarg-signing-key.asc

GLFW_TMP_DIR=$TMP_DIR/glfw-$GLFW_VERSION.bin.WIN64
GLFW_TMP_ZIP=$TMP_DIR/glfw-$GLFW_VERSION.bin.WIN64.zip

VULKAN_1_LIB=$TMP_DIR/vulkan-1.lib

# GLFW Windows library copy target
GLFW_WINDOWS_LIB=$GLFW_TMP_DIR/lib-mingw-w64/libglfw3.a

# GLFW install directory
GLFW_PATH="/usr/include/GLFW"

# URL required for downloading Vulkan windows library
VULKAN_LIB_URL="https://raw.githubusercontent.com/SaschaWillems/Vulkan/master/libs/vulkan/vulkan-1.lib"

# Vulkan install path
VULKAN_PATH=/usr/include/vulkan

# GLM install path
GLM_PATH=/usr/include/glm

# Updating package list
update_package_list() {
	echo "Updating the package list..."
	sudo apt update
}

# Adding the LunarG repository
add_LunarG_repository() {
	echo "Installing wget" # Necessary for the first exctracts, will be handy later on
	sudo apt install wget

	echo "Adding the LunarG signing key..."
	wget -q "$GPG_KEY_URL" -O $LUNARG_TMP_KEY

	sudo gpg --dearmor -o $LUNARG_GPG $LUNARG_TMP_KEY
	rm $LUNARG_TMP_KEY

	echo "Adding the LunarG Vulkan repository..."
	sudo wget -q "$REPO_URL" -O $LUNARG_REPOSITORY_LIST
}

# Instaling required packages
install_packages() {
	echo "Installing Vulkan SDK and additional libraries..."
	for package in "${PACKAGES[@]}"; do
		sudo apt install -y "$package"
	done
}

# Installing GLFW windows binaries
install_GLFW_for_MinGW() {
	echo "Downloading GLFW..."
	wget -q "$GLFW_URL" -O $GLFW_TMP_ZIP

	echo "Extracting GLFW..."
	unzip -q $GLFW_TMP_ZIP -d $TMP_DIR

	echo "Copying libglfw3.a to $MINGW_W64_LIB_DIR..."
	sudo cp $GLFW_WINDOWS_LIB "$MINGW_W64_LIB_DIR"

	echo "Copying GLFW headers from $GLFW_PATH to $MINGW_W64_INCLUDE_DIR..."
	sudo cp -r $GLFW_PATH $MINGW_W64_INCLUDE_DIR

	echo "Cleaning up..."
	rm -rf $GLFW_TMP_DIR $GLFW_TMP_ZIP
}

# Installing Vulkan windows binaries
install_Vulkan_for_MinGW() {
	echo "Downloading Vulkan library..."
	wget -q "$VULKAN_LIB_URL" -O $VULKAN_1_LIB

	echo "Copying vulkan-1.lib to $MINGW_W64_LIB_DIR..."
	sudo cp $VULKAN_1_LIB "$MINGW_W64_LIB_DIR"

	echo "Copying Vulkan headers from $VULKAN_PATH to $MINGW_W64_INCLUDE_DIR..."
	sudo cp -r $VULKAN_PATH $MINGW_W64_INCLUDE_DIR

	echo "Cleaning up..."
	rm $VULKAN_1_LIB
}

copy_GLM_for_MinGW() {
	echo "Copying GLM headers from $GLM_PATH to $MINGW_W64_INCLUDE_DIR..."
	sudo cp -r $GLM_PATH $MINGW_W64_INCLUDE_DIR
}

# Executing functions
update_package_list # First time to ensure wget is installable (suited for new installations, where apt update hasn't been executed yet)
add_LunarG_repository
update_package_list # Second time to account for the newly added LunarG repository
install_packages
install_GLFW_for_MinGW
install_Vulkan_for_MinGW
copy_GLM_for_MinGW

echo "Installation completed successfully!"

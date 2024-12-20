# Compilers
SHADERS_COMPILER = glslc
WINDOWS_COMPILER = x86_64-w64-mingw32-g++
LINUX_COMPILER = g++

# Flags
CFLAGS = -std=c++23 -O2
WINDOWS_LDFLAGS = -static -lglfw3 -lvulkan-1 -luser32 -lgdi32 -lwinmm
LINUX_LDFLAGS = -lglfw -lvulkan

# Directories
BUILD_DIR = build
WINDOWS_BUILD_DIR = $(BUILD_DIR)/windows_x64
LINUX_BUILD_DIR = $(BUILD_DIR)/linux_x64

SOURCE_DIR = src

SHADERS_DIR = $(SOURCE_DIR)/shaders
SHADERS_BUILD_DIR = $(BUILD_DIR)/shaders

WINDOWS_SHADERS_BUILD_DIR = $(WINDOWS_BUILD_DIR)/shaders
LINUX_SHADERS_BUILD_DIR = $(LINUX_BUILD_DIR)/shaders

# Targets
WINDOWS_TARGET = $(WINDOWS_BUILD_DIR)/engine.exe
LINUX_TARGET = $(LINUX_BUILD_DIR)/engine

# Wildcards
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS = $(wildcard $(SOURCE_DIR)/include/*.hpp)

VERT_SHADERS = $(wildcard $(SHADERS_DIR)/*.vert)
FRAG_SHADERS = $(wildcard $(SHADERS_DIR)/*.frag)

# Compilation targets
all: compile_shaders windows_x64 linux_x64

# Windows target with shader compilation
windows_x64: copy_shaders_windows $(WINDOWS_TARGET)

$(WINDOWS_TARGET): $(SOURCES) $(HEADERS)
	@mkdir -p $(WINDOWS_BUILD_DIR)
	$(WINDOWS_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(WINDOWS_LDFLAGS)

# Linux target with shader compilation
linux_x64: copy_shaders_linux $(LINUX_TARGET)

$(LINUX_TARGET): $(SOURCES) $(HEADERS)
	@mkdir -p $(LINUX_BUILD_DIR)
	$(LINUX_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(LINUX_LDFLAGS)

# Shader compilation
compile_shaders: $(patsubst $(SHADERS_DIR)/%.vert,$(SHADERS_BUILD_DIR)/%.vert.spv,$(VERT_SHADERS)) $(patsubst $(SHADERS_DIR)/%.frag,$(SHADERS_BUILD_DIR)/%.frag.spv,$(FRAG_SHADERS))
recompile_shaders: compile_shaders copy_shaders_windows copy_shaders_linux

# Copying compiled shaders to the Windows build directory
copy_shaders_windows: compile_shaders
	@mkdir -p $(WINDOWS_SHADERS_BUILD_DIR)
	@cp $(SHADERS_BUILD_DIR)/*.spv $(WINDOWS_SHADERS_BUILD_DIR)/

# Copying compiled shaders to the Linux build directory
copy_shaders_linux: compile_shaders
	@mkdir -p $(LINUX_SHADERS_BUILD_DIR)
	@cp $(SHADERS_BUILD_DIR)/*.spv $(LINUX_SHADERS_BUILD_DIR)/

# Compilation of vertex shaders
$(SHADERS_BUILD_DIR)/%.vert.spv: $(SHADERS_DIR)/%.vert
	@mkdir -p $(SHADERS_BUILD_DIR)
	$(SHADERS_COMPILER) $< -o $@

# Compilation of fragment shaders
$(SHADERS_BUILD_DIR)/%.frag.spv: $(SHADERS_DIR)/%.frag
	@mkdir -p $(SHADERS_BUILD_DIR)
	$(SHADERS_COMPILER) $< -o $@

.PHONY: all windows_x64 linux_x64 recompile_shaders clean clean_shaders

# Cleanup
clean:
	rm -rf $(BUILD_DIR)

clean_shaders:
	rm -rf $(SHADERS_BUILD_DIR) $(WINDOWS_SHADERS_BUILD_DIR) $(LINUX_SHADERS_BUILD_DIR)

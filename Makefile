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
SHADERS_COMPILED_DIR = $(SHADERS_DIR)/compiled

SHADERS_WINDOWS_BUILD_DIR = $(WINDOWS_BUILD_DIR)/shaders
SHADERS_LINUX_BUILD_DIR = $(LINUX_BUILD_DIR)/shaders

# Targets
TARGET_WINDOWS = $(WINDOWS_BUILD_DIR)/engine.exe
TARGET_LINUX = $(LINUX_BUILD_DIR)/engine

# Wildcards
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS = $(wildcard $(SOURCE_DIR)/include/*.hpp)

VERT_SHADERS = $(wildcard $(SHADERS_DIR)/*.vert)
FRAG_SHADERS = $(wildcard $(SHADERS_DIR)/*.frag)

# Compilation targets
all: windows_x64 linux_x64 copy_shaders_windows copy_shaders_linux
windows_x64: $(TARGET_WINDOWS)
linux_x64: $(TARGET_LINUX)
compile_shaders: $(patsubst $(SHADERS_DIR)/%.vert,$(SHADERS_COMPILED_DIR)/%.vert.spv,$(VERT_SHADERS)) $(patsubst $(SHADERS_DIR)/%.frag,$(SHADERS_COMPILED_DIR)/%.frag.spv,$(FRAG_SHADERS))

# Compilation of the Windows target
$(TARGET_WINDOWS): $(SOURCES) $(HEADERS)
	@mkdir -p $(WINDOWS_BUILD_DIR)
	$(WINDOWS_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(WINDOWS_LDFLAGS)

# Compilation of the Linux target
$(TARGET_LINUX): $(SOURCES) $(HEADERS)
	@mkdir -p $(LINUX_BUILD_DIR)
	$(LINUX_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(LINUX_LDFLAGS)

# Compilation of vertex shaders
$(SHADERS_COMPILED_DIR)/%.vert.spv: $(SHADERS_DIR)/%.vert
	@mkdir -p $(SHADERS_COMPILED_DIR)
	$(SHADERS_COMPILER) $< -o $@

# Compilation of fragment shaders
$(SHADERS_COMPILED_DIR)/%.frag.spv: $(SHADERS_DIR)/%.frag
	@mkdir -p $(SHADERS_COMPILED_DIR)
	$(SHADERS_COMPILER) $< -o $@

# Copying compiled shaders to the Windows build directory
copy_shaders_windows: compile_shaders
	@mkdir -p $(SHADERS_WINDOWS_BUILD_DIR)
	@cp $(SHADERS_COMPILED_DIR)/*.spv $(SHADERS_WINDOWS_BUILD_DIR)/

# Copying compiled shaders to the Linux build directory
copy_shaders_linux: compile_shaders
	@mkdir -p $(SHADERS_LINUX_BUILD_DIR)
	@cp $(SHADERS_COMPILED_DIR)/*.spv $(SHADERS_LINUX_BUILD_DIR)/

.PHONY: all windows_x64 linux_x64 compile_shaders copy_shaders_windows copy_shaders_linux clean

# Cleanup
clean:
	rm -rf $(BUILD_DIR) $(SHADERS_COMPILED_DIR)

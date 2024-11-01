WINDOWS_COMPILER = x86_64-w64-mingw32-g++
LINUX_COMPILER = g++

CFLAGS = -std=c++23 -O2
WINDOWS_LDFLAGS = -static -mwindows -lglfw3 -lvulkan-1 -luser32 -lgdi32 -lwinmm
LINUX_LDFLAGS = -lglfw -lvulkan

TARGET_WINDOWS = build/windows_x64/engine.exe
TARGET_LINUX = build/linux_x64/engine

SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/include/*.hpp)

all: linux_x64 windows_x64
linux_x64: $(TARGET_LINUX)
windows_x64: $(TARGET_WINDOWS)

$(TARGET_LINUX): $(SOURCES) $(HEADERS)
	@mkdir -p build/linux_x64
	$(LINUX_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(LINUX_LDFLAGS)

$(TARGET_WINDOWS): $(SOURCES) $(HEADERS)
	@mkdir -p build/windows_x64
	$(WINDOWS_COMPILER) $(CFLAGS) -o $@ $(SOURCES) $(WINDOWS_LDFLAGS)

.PHONY: all linux windows clean

clean:
	rm -rf build

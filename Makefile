CC = em++
CFLAGS += -Os -s USE_GLFW=3 -s USE_SDL=2 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -O1 -Wall -Werror -std=c++20 -D_DEFAULT_SOURCE -Wno-missing-braces
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include
LDLIBS = $(RAYLIB_PATH)/lib/libraylib.a
PLATFORM=PLATFORM_DESKTOP


all: main

main: cube.cpp
	@mkdir -p build
	$(CC) -o build/cube.js cube.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D $(PLATFORM)
	$(CC) -o build/vectors.js vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Vectors
clean:
	@rm build/*
	@rmdir build
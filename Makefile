CC = em++
CFLAGS += -Os -s USE_GLFW=3 -s USE_SDL=2 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -O1 -Wall -std=c++17 -D_DEFAULT_SOURCE -Wno-missing-braces
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include -I$(PHYCOUB_PATH)/include
LDLIBS = $(RAYLIB_PATH)/lib/libraylib.a $(PHYCOUB_PATH)/lib/libphycoub.a
PLATFORM = PLATFORM_DESKTOP

RAYLIB_PATH = raylib_webassembly
PHYCOUB_PATH = phycoub_webassembly

all: main

main: cube.cpp
	@mkdir -p build
	$(CC) -o build/cube.js cube.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D $(PLATFORM)
	$(CC) -o build/shoot.js vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D OBJECT=0 -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Shoot
	$(CC) -o build/magnetic.js vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D OBJECT=1 -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Magnetic
	$(CC) -o build/electric.js vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D OBJECT=2 -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Electric
clean:
	@rm build/*
	@rmdir build
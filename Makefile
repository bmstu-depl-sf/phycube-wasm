ifeq ($(PLATFORM), PLATFORM_WEB)
	CC = em++
	CFLAGS += -Os -s USE_GLFW=3 -s USE_SDL=2 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY
	INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include
	LDLIBS = $(RAYLIB_PATH)/lib/libraylib.a
	OUTPUT=build/index.html
else
	CC = g++
	ifeq ($(shell uname), Darwin)
		CFLAGS += -framework IOKit -framework Cocoa -framework OpenGL
	endif
	INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include
	LDLIBS = $(RAYLIB_PATH)/lib/libraylib.a
	PLATFORM=PLATFORM_DESKTOP
	OUTPUT=build/app.exe
endif

CFLAGS += -O1 -Wall -Werror -std=c++20 -D_DEFAULT_SOURCE -Wno-missing-braces

all: main

main: main.cpp
	@mkdir -p build
	$(CC) -o $(OUTPUT) $< $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D $(PLATFORM)
clean:
	@rm build/*
	@rmdir build
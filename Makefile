CC = em++
CFLAGS += src/functions.cpp -Os -s USE_GLFW=3 -s USE_SDL=2 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH -O1 -Wall -std=c++17 -D_DEFAULT_SOURCE -Wno-missing-braces -g
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include -I$(PHYCOUB_PATH)/include
LDLIBS = $(RAYLIB_PATH)/lib/libraylib.a $(PHYCOUB_PATH)/lib/libphycoub.a
PLATFORM = PLATFORM_DESKTOP

# Пути к папкам со скомпилированными для Wasm библиотеками
RAYLIB_PATH = raylib_webassembly
PHYCOUB_PATH = phycoub_webassembly

# Частота зарождения новых частиц
BORN_PERIOD=250
# Параметры каждой частицы: энергия, масса, заряд, название
PARTICLE_SPECS={{1e-17, 9.1e-31, -1.6e-19, "Электроны"}, {1e-15, 1.7e-27, 1.6e-19, "Протоны"}, {1e-15, 6.6e-27, 3.2e-19, "Альфа-частицы"}}

all: main

main: src/cube.cpp src/chamber.cpp src/functions.cpp
	@mkdir -p build
	$(CC) -o build/cube.js src/cube.cpp src/chamber.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -s EXPORTED_RUNTIME_METHODS=wasmMemory -D $(PLATFORM) -D BORN_PERIOD=$(BORN_PERIOD) -D PARTICLE_SPECS='$(PARTICLE_SPECS)'
	$(CC) -o build/magnetic.js src/vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D OBJECT=1 -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Magnetic
	$(CC) -o build/electric.js src/vector.cpp $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D OBJECT=2 -D $(PLATFORM) -s MODULARIZE -sEXPORT_NAME=Electric
clean:
	@rm build/*
	@rmdir build
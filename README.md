## PhyCube Wasm

Инструкция по сборке:

1. Скачать и распаковать [стабильные сборки](https://github.com/raysan5/raylib/releases/tag/4.5.0) библиотеки для нужных платформ (WebAssembly/Desktop).

2. Для сборки в WebAssembly установить Emscripten:
- MacOS: `brew install emscripten`
- Linux: `apt install emscripten`
- Windows: [инструкция](https://emscripten.org/docs/getting_started/downloads.html)

3. Собрать пример (требуется gcc/clang и make):
- Desktop: `RAYLIB_PATH=/path/to/raylib make`
- WebAssembly: `RAYLIB_PATH=/path/to/raylib PLATFORM=PLATFORM_WEB make`

4. (Для WebAssembly) Запустить пример можно командой `emrun build/index.html` или любым другим локальным сервером, например, `python -m http.server -d build`.
## PhyCube Wasm

Инструкция по сборке:

1. Скачать и распаковать [стабильную сборку](https://github.com/raysan5/raylib/releases/tag/4.5.0) библиотеки для WebAssembly.

2. Установить Emscripten (необязательно, стабильная сборка уже включена в репозиторий):
- MacOS: `brew install emscripten`
- Linux: `apt install emscripten`
- Windows: [инструкция](https://emscripten.org/docs/getting_started/downloads.html)

3. Собрать пример (требуется gcc/clang и make):
`make`

4. Запустить пример можно любым локальным сервером, например, `python -m http.server`.
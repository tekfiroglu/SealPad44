# SealPad44

A minimal text editor built with raylib, written in C.

## Features
- Basic text editing with a scrollable text buffer
- Open and save files via a custom path input field
- Mouse-wheel scrolling (hold `Shift` for horizontal scroll)
- Seal noise
- i love seals

## Requirements
- [raylib](https://www.raylib.com/)
- GCC or Clang
- A TTF font

## Build

```bash
gcc main.c -o sealpad44 -lraylib -lm -lpthread -ldl -lrt -lX11
```

Or with `pkg-config`:

```bash
gcc main.c -o sealpad44 $(pkg-config --libs --cflags raylib)
```

## Usage

```bash
./sealpad44
```

- Click the **path** field to type a file path, press `Enter` to confirm
- Click **save** (top-right) to write the buffer to the given path
- Click **open** (below save) to load a file into the buffer
- Scroll with the mouse wheel, hold `Shift` to scroll horizontally

## Known Issues
- No buffer overflow protection yet — very long text/paths can crash the app
- Still early WIP, expect bugs

## Roadmap
- [ ] Bundle font/sound as local resources
- [ ] Add buffer bounds checking
- [ ] Better UI feedback (unsaved changes indicator, etc.)

## License
This project is licensed under the MIT License

## Contributing
PRs and issues welcome, this is a learning project so feedback appreciated.

# Idea Pad Manager
A TUI-based Lenovo IdeaPad manager for Linux.

Note: This project is an independent, open-source tool and is not affiliated with, authorized, or endorsed by Lenovo.

# Demo
![Demo](https://github.com/user-attachments/assets/d8a32f47-61a6-440c-a2b7-7364564c6dbd)

# Dependencies
- **C++23** compiler (e.g., clang, gcc)
- **CMake** (>= 3.15)
- **Ninja** - Build system
- **FTXUI** (v6.1.9) - Terminal UI library (fetched via CMake FetchContent)
- **spdlog** (v1.17.0) - Logging library (fetched via CMake FetchContent)

For development:
- **clang-format** - Code formatting
- **clang-tidy** - Static analysis

# Development Environment
# Installation / Building / Running

## Build
```bash
./scripts/build.sh
```

Or manually:
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja -C build
```

## Run
```bash
./scripts/run.sh
```

Or manually:
```bash
./build/IdeaPadManager
```

## Development Scripts
```bash
# Format code
./scripts/format.sh

# Run clang-tidy and fix issues
./scripts/tidy.sh
```

### Using Nix Flake
This project uses a [Nix flake](https://nixos.org/manual/nix/stable/command-ref/new-cli/nix3-flake.html) for the development environment.

```bash
nix develop --no-pure-eval
```

# TODO
 - Make it more responsive to the change in terminal dimentions

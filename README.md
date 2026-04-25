# b64decode

A standalone Base64 file decoder written in modern C++17. The decoder is implemented from scratch — no third-party crypto or encoding libraries — using a 256-entry lookup table and bit-level reassembly to convert a Base64 stream into raw bytes.

## Why this project

The decoder demonstrates several portfolio-relevant practices in a small, self-contained codebase:

- **Library-first design.** All Base64 logic lives behind a public `b64::` namespace API in `include/base64_decoder.hpp`; the CLI in `src/main.cpp` is a thin shell over `b64::decode_stream`.
- **Constant-time character lookup.** A `constexpr` 256-byte alphabet table replaces the linear scan used in the original implementation.
- **Padding-aware streaming.** The decoder consumes arbitrary-sized Base64 streams, tolerates whitespace, honours `=` padding, and reports malformed (truncated) input instead of writing past the valid output length — a real bug that existed in the legacy version.
- **Unit-tested.** GoogleTest is fetched at configure time via CMake `FetchContent`. The suite covers the full alphabet, single/double padding, whitespace, truncation rejection, and a full 0–255 byte round-trip.
- **Documented.** Every public symbol carries a Doxygen block; the included `Doxyfile` produces HTML reference docs in one command.

## Build and run

Requires CMake ≥ 3.14 and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build -j
./build/b64decode data/test.base64 decoded.bin
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Generate API docs

```bash
doxygen docs/Doxyfile
# Open docs/build/html/index.html
```

## Project layout

```
b64decode/
├── CMakeLists.txt                      # Build, FetchContent for GoogleTest
├── include/
│   └── base64_decoder.hpp              # Public namespace API + Doxygen
├── src/
│   ├── base64_decoder.cpp              # Lookup table, block decode, stream decode
│   └── main.cpp                        # CLI: argv parsing, file I/O, exit codes
├── tests/
│   └── test_base64_decoder.cpp         # GoogleTest cases
├── docs/
│   └── Doxyfile                        # Doxygen configuration
└── data/
    └── test.base64                     # Sample encoded fixture
```

## CLI

```
b64decode <input_file> <output_file>
```

Exit codes: `0` success, `1` usage error, `2` filesystem error, `3` malformed input.

## Origins

This project began as a homework assignment built in C++ Builder 6 for Windows. It was rewritten on the `refactor/clean-architecture` branch into a portable, tested, documented portfolio piece. The original Borland project files and Polish-language identifiers have been removed; the decoding algorithm is otherwise unchanged.

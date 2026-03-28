# b64decode

A command-line **Base64 file decoder** built with **C++ Builder 6**, implementing the decoding algorithm from scratch using bit manipulation and stream I/O.

![C++](https://img.shields.io/badge/C%2B%2B-Builder%206-blue)
![Windows](https://img.shields.io/badge/Platform-Windows-0078D6)
![Base64](https://img.shields.io/badge/Encoding-Base64-orange)

## Features

- **Full Base64 decoding** from RFC 2045 alphabet (A-Z, a-z, 0-9, +, /)
- **Bit-level reassembly** -- extracts 6-bit values from 4 input characters and reconstructs 3 output bytes using bitwise shift and OR operations
- **File-to-file processing** -- reads encoded input from one file, writes decoded binary to another
- **Whitespace-tolerant** -- skips non-Base64 characters (newlines, spaces) during decoding
- **Stream-based I/O** -- uses `std::fstream` for sequential character-by-character reading
- **Lookup table decoding** -- maps each Base64 character to its 6-bit value via the standard alphabet array
- **Error handling** -- validates argument count and file open status with descriptive error messages

## How It Works

The decoder processes input in groups of 4 Base64 characters at a time:

1. Each character is mapped to its 6-bit index in the Base64 alphabet
2. The four 6-bit values (24 bits total) are reassembled into 3 output bytes:
   - Byte 1: `(val[0] << 2) | (val[1] >> 4)`
   - Byte 2: `(val[1] << 4) | (val[2] >> 2)`
   - Byte 3: `(val[2] << 6) | val[3]`
3. Output bytes are written to the destination file

## Dependencies

| Component | Purpose |
|-----------|---------|
| C++ Builder 6 | IDE and compiler |
| VCL / RTL | Standard runtime libraries |

## Building

### With C++ Builder 6

1. Open `b64decode.bpr` in the C++ Builder IDE.
2. Press **F9** to build and run.

### With any C++ compiler

The source code is standard C++ and can be compiled independently:

```bash
g++ -o b64decode main.cpp
```

## Usage

```bash
b64decode <input_file> <output_file>
```

**Example:**

```bash
b64decode test.base64 output.bin
```

The program expects exactly 2 arguments. It will report an error if files cannot be opened.

## Project Structure

```
b64decode/
├── main.cpp          # Base64 decoder implementation
├── b64decode.bpr     # C++ Builder 6 project file
├── b64decode.bpf     # Project form file
├── b64decode.res     # Compiled resource file
├── test.base64       # Sample Base64 encoded input
└── wyj.txt           # Sample decoded output
```

## License

This project is provided as-is for educational purposes.

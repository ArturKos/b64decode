#include "base64_decoder.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace {

constexpr int kExitSuccess = 0;
constexpr int kExitUsage = 1;
constexpr int kExitIoError = 2;
constexpr int kExitDecodeError = 3;

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <input_file> <output_file>\n"
              << "  Decodes a Base64-encoded file into raw bytes.\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage(argc > 0 ? argv[0] : "b64decode");
        return kExitUsage;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    std::ifstream input(input_path, std::ios::binary);
    if (!input) {
        std::cerr << "Cannot open input file: " << input_path << '\n';
        return kExitIoError;
    }

    std::ofstream output(output_path, std::ios::binary | std::ios::trunc);
    if (!output) {
        std::cerr << "Cannot open output file: " << output_path << '\n';
        return kExitIoError;
    }

    if (!b64::decode_stream(input, output)) {
        std::cerr << "Malformed Base64 input.\n";
        return kExitDecodeError;
    }

    return kExitSuccess;
}

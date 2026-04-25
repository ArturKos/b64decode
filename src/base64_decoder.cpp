#include "base64_decoder.hpp"

#include <array>
#include <cctype>
#include <climits>
#include <cstdint>

namespace b64 {

namespace {

constexpr std::size_t kLookupTableSize = 1u << CHAR_BIT;

constexpr std::array<std::uint8_t, kLookupTableSize> build_decode_table() {
    std::array<std::uint8_t, kLookupTableSize> table{};
    for (auto& slot : table) {
        slot = 0xFFu;
    }
    std::uint8_t value = 0;
    for (char character = 'A'; character <= 'Z'; ++character) {
        table[static_cast<unsigned char>(character)] = value++;
    }
    for (char character = 'a'; character <= 'z'; ++character) {
        table[static_cast<unsigned char>(character)] = value++;
    }
    for (char character = '0'; character <= '9'; ++character) {
        table[static_cast<unsigned char>(character)] = value++;
    }
    table[static_cast<unsigned char>('+')] = value++;
    table[static_cast<unsigned char>('/')] = value;
    return table;
}

constexpr auto kDecodeTable = build_decode_table();

}  // namespace

bool is_base64_character(char character) noexcept {
    return kDecodeTable[static_cast<unsigned char>(character)] != 0xFFu;
}

int decode_character(char character) noexcept {
    const std::uint8_t value = kDecodeTable[static_cast<unsigned char>(character)];
    return value == 0xFFu ? kInvalidCharacter : static_cast<int>(value);
}

std::size_t decode_block(const char encoded[kQuartetSize],
                         std::uint8_t decoded[kTripletSize],
                         std::size_t padding_count) noexcept {
    const std::uint8_t value0 = kDecodeTable[static_cast<unsigned char>(encoded[0])];
    const std::uint8_t value1 = kDecodeTable[static_cast<unsigned char>(encoded[1])];
    const std::uint8_t value2 = kDecodeTable[static_cast<unsigned char>(encoded[2])];
    const std::uint8_t value3 = kDecodeTable[static_cast<unsigned char>(encoded[3])];

    decoded[0] = static_cast<std::uint8_t>((value0 << 2) | (value1 >> 4));
    decoded[1] = static_cast<std::uint8_t>((value1 << 4) | (value2 >> 2));
    decoded[2] = static_cast<std::uint8_t>((value2 << 6) | value3);

    return kTripletSize - padding_count;
}

namespace {

bool fill_quartet(std::istream& input,
                  char quartet[kQuartetSize],
                  std::size_t& filled,
                  std::size_t& padding_count) {
    filled = 0;
    padding_count = 0;
    char character = 0;
    while (filled < kQuartetSize && input.get(character)) {
        if (character == kPaddingCharacter) {
            quartet[filled++] = 'A';
            ++padding_count;
        } else if (is_base64_character(character)) {
            if (padding_count > 0) {
                return false;
            }
            quartet[filled++] = character;
        }
    }
    if (filled == 0) {
        return true;
    }
    if (filled != kQuartetSize) {
        return false;
    }
    return padding_count <= 2;
}

}  // namespace

bool decode_stream(std::istream& input, std::ostream& output) {
    char quartet[kQuartetSize];
    std::uint8_t triplet[kTripletSize];
    std::size_t filled = 0;
    std::size_t padding_count = 0;

    while (true) {
        if (!fill_quartet(input, quartet, filled, padding_count)) {
            return false;
        }
        if (filled == 0) {
            return true;
        }
        const std::size_t produced = decode_block(quartet, triplet, padding_count);
        output.write(reinterpret_cast<const char*>(triplet),
                     static_cast<std::streamsize>(produced));
        if (!output) {
            return false;
        }
        if (padding_count > 0) {
            return true;
        }
    }
}

}  // namespace b64

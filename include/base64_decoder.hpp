#pragma once

#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>

namespace b64 {

/**
 * @brief Number of distinct characters in the standard Base64 alphabet.
 */
constexpr std::size_t kAlphabetSize = 64;

/**
 * @brief Number of encoded characters that decode to one output triplet.
 */
constexpr std::size_t kQuartetSize = 4;

/**
 * @brief Number of decoded bytes produced by a full quartet.
 */
constexpr std::size_t kTripletSize = 3;

/**
 * @brief Padding character used to align the final quartet.
 */
constexpr char kPaddingCharacter = '=';

/**
 * @brief Sentinel returned by decode_character() for non-alphabet input.
 */
constexpr int kInvalidCharacter = -1;

/**
 * @brief Test whether a byte belongs to the standard RFC 4648 Base64 alphabet.
 * @param character Byte to inspect.
 * @return true if the character is one of A-Z, a-z, 0-9, '+' or '/'.
 */
bool is_base64_character(char character) noexcept;

/**
 * @brief Map a Base64 alphabet character to its 6-bit value.
 * @param character Byte expected to belong to the alphabet.
 * @return Value in [0, 63] for valid input, kInvalidCharacter otherwise.
 */
int decode_character(char character) noexcept;

/**
 * @brief Decode a single 4-character block into up to 3 bytes.
 * @param encoded Quartet of alphabet characters; trailing positions may hold '=' padding.
 * @param decoded Output buffer of at least 3 bytes.
 * @param padding_count Number of padding characters at the end of @p encoded (0, 1 or 2).
 * @return Number of bytes written to @p decoded (1, 2 or 3 = kTripletSize - padding_count).
 */
std::size_t decode_block(const char encoded[kQuartetSize],
                         std::uint8_t decoded[kTripletSize],
                         std::size_t padding_count) noexcept;

/**
 * @brief Decode a Base64 stream into raw bytes.
 *
 * Non-alphabet characters other than '=' are skipped, allowing the input to contain
 * line breaks or whitespace. Decoding stops at end of stream or on the first padding
 * sequence that completes a quartet.
 *
 * @param input  Stream positioned at the first encoded byte.
 * @param output Stream that receives the decoded bytes.
 * @return true if the stream was consumed without encountering a malformed quartet.
 */
bool decode_stream(std::istream& input, std::ostream& output);

}  // namespace b64

#include "base64_decoder.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>

namespace {

std::string decode_to_string(const std::string& encoded) {
    std::istringstream input(encoded);
    std::ostringstream output;
    EXPECT_TRUE(b64::decode_stream(input, output));
    return output.str();
}

bool decode_succeeds(const std::string& encoded, std::string& decoded) {
    std::istringstream input(encoded);
    std::ostringstream output;
    const bool ok = b64::decode_stream(input, output);
    decoded = output.str();
    return ok;
}

}  // namespace

TEST(IsBase64Character, AcceptsFullAlphabet) {
    for (char c = 'A'; c <= 'Z'; ++c) EXPECT_TRUE(b64::is_base64_character(c));
    for (char c = 'a'; c <= 'z'; ++c) EXPECT_TRUE(b64::is_base64_character(c));
    for (char c = '0'; c <= '9'; ++c) EXPECT_TRUE(b64::is_base64_character(c));
    EXPECT_TRUE(b64::is_base64_character('+'));
    EXPECT_TRUE(b64::is_base64_character('/'));
}

TEST(IsBase64Character, RejectsNonAlphabet) {
    EXPECT_FALSE(b64::is_base64_character('='));
    EXPECT_FALSE(b64::is_base64_character(' '));
    EXPECT_FALSE(b64::is_base64_character('\n'));
    EXPECT_FALSE(b64::is_base64_character('-'));
    EXPECT_FALSE(b64::is_base64_character('@'));
}

TEST(DecodeCharacter, MapsAlphabetToContiguousValues) {
    EXPECT_EQ(b64::decode_character('A'), 0);
    EXPECT_EQ(b64::decode_character('Z'), 25);
    EXPECT_EQ(b64::decode_character('a'), 26);
    EXPECT_EQ(b64::decode_character('z'), 51);
    EXPECT_EQ(b64::decode_character('0'), 52);
    EXPECT_EQ(b64::decode_character('9'), 61);
    EXPECT_EQ(b64::decode_character('+'), 62);
    EXPECT_EQ(b64::decode_character('/'), 63);
}

TEST(DecodeCharacter, ReturnsSentinelForInvalid) {
    EXPECT_EQ(b64::decode_character('='), b64::kInvalidCharacter);
    EXPECT_EQ(b64::decode_character('\0'), b64::kInvalidCharacter);
    EXPECT_EQ(b64::decode_character('!'), b64::kInvalidCharacter);
}

TEST(DecodeStream, EmptyInputProducesEmptyOutput) {
    EXPECT_EQ(decode_to_string(""), "");
}

TEST(DecodeStream, ThreeByteAlignedInput) {
    EXPECT_EQ(decode_to_string("YWJjZGVm"), "abcdef");
}

TEST(DecodeStream, OneTrailingByteUsesDoublePadding) {
    EXPECT_EQ(decode_to_string("YQ=="), "a");
}

TEST(DecodeStream, TwoTrailingBytesUseSinglePadding) {
    EXPECT_EQ(decode_to_string("YWI="), "ab");
}

TEST(DecodeStream, IgnoresWhitespaceBetweenCharacters) {
    EXPECT_EQ(decode_to_string("YWJj\nZGVm\n"), "abcdef");
    EXPECT_EQ(decode_to_string(" Y W J j Z G V m "), "abcdef");
}

TEST(DecodeStream, DecodesFullAlphabetOfPrintableCharacters) {
    const std::string encoded =
        "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXowMTIzNDU2Nzg5";
    EXPECT_EQ(decode_to_string(encoded), "abcdefghijklmnopqrstuvwxyz0123456789");
}

TEST(DecodeStream, DecodesAllByteValues) {
    std::string raw;
    raw.reserve(256);
    for (int byte = 0; byte < 256; ++byte) {
        raw.push_back(static_cast<char>(byte));
    }
    const std::string encoded =
        "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4v"
        "MDEyMzQ1Njc4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5f"
        "YGFiY2RlZmdoaWprbG1ub3BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6P"
        "kJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+wsbKztLW2t7i5uru8vb6/"
        "wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g4eLj5OXm5+jp6uvs7e7v"
        "8PHy8/T19vf4+fr7/P3+/w==";
    std::string decoded;
    ASSERT_TRUE(decode_succeeds(encoded, decoded));
    EXPECT_EQ(decoded, raw);
}

TEST(DecodeStream, RejectsTruncatedQuartet) {
    std::string decoded;
    EXPECT_FALSE(decode_succeeds("YWJ", decoded));
}

TEST(DecodeStream, StopsAtFirstCompleteQuartetWithPadding) {
    std::string decoded;
    ASSERT_TRUE(decode_succeeds("YWI=YWJj", decoded));
    EXPECT_EQ(decoded, "ab");
}

TEST(DecodeBlock, SubtractsPaddingFromOutputLength) {
    char encoded[4] = {'Y', 'Q', 'A', 'A'};
    std::uint8_t decoded[3] = {};
    EXPECT_EQ(b64::decode_block(encoded, decoded, 2), 1u);
    EXPECT_EQ(decoded[0], 'a');
}

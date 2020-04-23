// MIT License
//
// Copyright (c) 2018-2019 Lennart Braun
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "gtest/gtest.h"

#include "test_constants.h"

#include "crypto/aes/aesni_primitives.h"

// Test vectors from NIST FIPS 197, Appendix A

TEST(aesni128, key_expansion) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  const std::array<std::uint8_t, aes_round_keys_size_128> expected_round_keys = {
      0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f,
      0x3c, 0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c,
      0x76, 0x05, 0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73,
      0x59, 0xf6, 0x7f, 0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44,
      0x6d, 0x7a, 0x88, 0x3b, 0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25,
      0x3b, 0xdb, 0x0b, 0xad, 0x00, 0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2,
      0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc, 0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb,
      0xf9, 0x86, 0x41, 0xca, 0x00, 0x93, 0xfd, 0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3,
      0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f, 0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba,
      0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f, 0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa,
      0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e, 0xd0, 0x14, 0xf9, 0xa8, 0xc9,
      0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());
  EXPECT_EQ(round_keys, expected_round_keys);
}

// Expected output generated with pyaes

TEST(aesni128, ctr_stream) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());

  alignas(aes_block_size) std::array<std::uint8_t, 8 * aes_block_size> output;
  const std::array<std::uint8_t, 8 * aes_block_size> expected_output = {
      0x7d, 0xf7, 0x6b, 0x0c, 0x1a, 0xb8, 0x99, 0xb3, 0x3e, 0x42, 0xf0, 0x47, 0xb9, 0x1b, 0x54,
      0x6f, 0x7e, 0x59, 0x37, 0x9b, 0x52, 0x33, 0x96, 0x9d, 0x25, 0xa5, 0xad, 0x2c, 0xe3, 0x35,
      0xcb, 0x3e, 0x1f, 0xb0, 0xc2, 0x3b, 0xd2, 0x09, 0xac, 0x91, 0x1e, 0xe3, 0xab, 0x8a, 0x2d,
      0x85, 0xeb, 0xcd, 0xc2, 0x4b, 0xfe, 0xa9, 0xb5, 0x60, 0xce, 0x46, 0xc7, 0x87, 0xe9, 0xed,
      0x29, 0xe7, 0x16, 0x0f, 0xcd, 0xa4, 0x3d, 0x7c, 0x6c, 0x56, 0xb6, 0x27, 0xa9, 0x69, 0x30,
      0xa1, 0xf0, 0xb9, 0x91, 0x6b, 0xc9, 0x36, 0xb3, 0x35, 0x1a, 0xc0, 0x01, 0xf7, 0x36, 0x16,
      0x9e, 0xb1, 0xa0, 0xb2, 0x02, 0xc0, 0x2e, 0xf9, 0x5b, 0xd9, 0x68, 0x83, 0xef, 0x66, 0x82,
      0xc2, 0xde, 0x66, 0xc7, 0x76, 0x3a, 0x24, 0x4c, 0x5a, 0x8b, 0xbf, 0x09, 0xe3, 0xc3, 0x8c,
      0x43, 0x57, 0x3d, 0x56, 0xc3, 0x3f, 0x83, 0xa9};
  for (size_t n = 1; n <= 8; ++n) {
    std::uint64_t counter = 0;
    aesni_ctr_stream_blocks_128(round_keys.data(), &counter, output.data(), n);
    EXPECT_TRUE(std::equal(std::begin(output), std::begin(output) + n * aes_block_size,
                           std::begin(expected_output)));
  }
}

TEST(aesni128, ctr_stream_unaligned) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());

  std::array<std::uint8_t, 8 * aes_block_size> output;
  const std::array<std::uint8_t, 8 * aes_block_size> expected_output = {
      0x7d, 0xf7, 0x6b, 0x0c, 0x1a, 0xb8, 0x99, 0xb3, 0x3e, 0x42, 0xf0, 0x47, 0xb9, 0x1b, 0x54,
      0x6f, 0x7e, 0x59, 0x37, 0x9b, 0x52, 0x33, 0x96, 0x9d, 0x25, 0xa5, 0xad, 0x2c, 0xe3, 0x35,
      0xcb, 0x3e, 0x1f, 0xb0, 0xc2, 0x3b, 0xd2, 0x09, 0xac, 0x91, 0x1e, 0xe3, 0xab, 0x8a, 0x2d,
      0x85, 0xeb, 0xcd, 0xc2, 0x4b, 0xfe, 0xa9, 0xb5, 0x60, 0xce, 0x46, 0xc7, 0x87, 0xe9, 0xed,
      0x29, 0xe7, 0x16, 0x0f, 0xcd, 0xa4, 0x3d, 0x7c, 0x6c, 0x56, 0xb6, 0x27, 0xa9, 0x69, 0x30,
      0xa1, 0xf0, 0xb9, 0x91, 0x6b, 0xc9, 0x36, 0xb3, 0x35, 0x1a, 0xc0, 0x01, 0xf7, 0x36, 0x16,
      0x9e, 0xb1, 0xa0, 0xb2, 0x02, 0xc0, 0x2e, 0xf9, 0x5b, 0xd9, 0x68, 0x83, 0xef, 0x66, 0x82,
      0xc2, 0xde, 0x66, 0xc7, 0x76, 0x3a, 0x24, 0x4c, 0x5a, 0x8b, 0xbf, 0x09, 0xe3, 0xc3, 0x8c,
      0x43, 0x57, 0x3d, 0x56, 0xc3, 0x3f, 0x83, 0xa9};
  for (size_t n = 1; n <= 8; ++n) {
    std::uint64_t counter = 0;
    aesni_ctr_stream_blocks_128_unaligned(round_keys.data(), &counter, output.data(), n);
    EXPECT_TRUE(std::equal(std::begin(output), std::begin(output) + n * aes_block_size,
                           std::begin(expected_output)));
  }
}

TEST(aesni128, ctr_stream_blockwise) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());

  std::array<std::uint8_t, 8 * aes_block_size> output;
  const std::array<std::uint8_t, 8 * aes_block_size> expected_output = {
      0x7d, 0xf7, 0x6b, 0x0c, 0x1a, 0xb8, 0x99, 0xb3, 0x3e, 0x42, 0xf0, 0x47, 0xb9, 0x1b, 0x54,
      0x6f, 0x7e, 0x59, 0x37, 0x9b, 0x52, 0x33, 0x96, 0x9d, 0x25, 0xa5, 0xad, 0x2c, 0xe3, 0x35,
      0xcb, 0x3e, 0x1f, 0xb0, 0xc2, 0x3b, 0xd2, 0x09, 0xac, 0x91, 0x1e, 0xe3, 0xab, 0x8a, 0x2d,
      0x85, 0xeb, 0xcd, 0xc2, 0x4b, 0xfe, 0xa9, 0xb5, 0x60, 0xce, 0x46, 0xc7, 0x87, 0xe9, 0xed,
      0x29, 0xe7, 0x16, 0x0f, 0xcd, 0xa4, 0x3d, 0x7c, 0x6c, 0x56, 0xb6, 0x27, 0xa9, 0x69, 0x30,
      0xa1, 0xf0, 0xb9, 0x91, 0x6b, 0xc9, 0x36, 0xb3, 0x35, 0x1a, 0xc0, 0x01, 0xf7, 0x36, 0x16,
      0x9e, 0xb1, 0xa0, 0xb2, 0x02, 0xc0, 0x2e, 0xf9, 0x5b, 0xd9, 0x68, 0x83, 0xef, 0x66, 0x82,
      0xc2, 0xde, 0x66, 0xc7, 0x76, 0x3a, 0x24, 0x4c, 0x5a, 0x8b, 0xbf, 0x09, 0xe3, 0xc3, 0x8c,
      0x43, 0x57, 0x3d, 0x56, 0xc3, 0x3f, 0x83, 0xa9};
  std::uint64_t counter = 0;
  for (size_t n = 0; n < 8; ++n) {
    aesni_ctr_stream_single_block_128_unaligned(round_keys.data(), &counter,
                                                output.data() + n * aes_block_size);
  }
  EXPECT_EQ(output, expected_output);
}

TEST(aesni128, tmmo_batch_4) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());

  alignas(aes_block_size) std::array<std::uint8_t, 4 * aes_block_size> output = {
      0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
      0x41, 0x41, 0x41, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
      0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43,
      0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x44, 0x44, 0x44, 0x44,
      0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44};
  const std::array<std::uint8_t, 4 * aes_block_size> expected_output = {
      0x56, 0x15, 0x69, 0x3f, 0xaf, 0x9c, 0xc4, 0x75, 0xf5, 0x31, 0xf0, 0x97, 0x48,
      0x1d, 0xa5, 0xe9, 0x46, 0x4a, 0x73, 0x47, 0x9d, 0x0c, 0x6b, 0xf6, 0x5e, 0xb3,
      0x96, 0x7a, 0x7f, 0xcc, 0x2b, 0x42, 0x68, 0x8e, 0x31, 0x78, 0xfc, 0x4a, 0xd7,
      0x26, 0x9f, 0x5a, 0xf8, 0x45, 0x74, 0x72, 0xb0, 0xf9, 0x6c, 0xe7, 0xb6, 0xd5,
      0x94, 0xfa, 0x76, 0x43, 0xad, 0x69, 0x10, 0xb9, 0x78, 0x6a, 0x01, 0x06};
  __uint128_t tweak = 0xdeadbeefdeadcafe;
  tweak <<= 64;
  tweak |= 0xbeefcafecafebeef;
  aesni_tmmo_batch_4(round_keys.data(), output.data(), tweak);
  EXPECT_EQ(output, expected_output);
}

TEST(aesni128, mmo_single) {
  std::array<std::uint8_t, aes_key_size_128> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                                    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  alignas(aes_block_size) std::array<std::uint8_t, aes_round_keys_size_128> round_keys;
  std::copy(std::begin(key), std::end(key), std::begin(round_keys));
  aesni_key_expansion_128(round_keys.data());

  alignas(aes_block_size) std::array<std::uint8_t, aes_block_size> output = {
      0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
      0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
  const std::array<std::uint8_t, aes_block_size> expected_output = {
      0x2d, 0x6b, 0x7e, 0x98, 0x7b, 0xe6, 0xf5, 0x56,
      0x84, 0x02, 0xcc, 0x67, 0xe5, 0x20, 0xd4, 0x58};
  aesni_mmo_single(round_keys.data(), output.data());
  EXPECT_EQ(output, expected_output);
}

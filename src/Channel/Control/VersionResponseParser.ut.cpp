// SPDX-License-Identifier: GPL-3.0-or-later
#include <array>
#include <gtest/gtest.h>
#include <aasdk/Channel/Control/VersionResponseParser.hpp>

namespace aasdk::channel::control {

TEST(VersionResponseParser, DecodesSixOneAndPreservesTrailingBytes) {
  const std::array<uint8_t, 9> payload{0x00, 0x06, 0x00, 0x01, 0x00, 0x00,
                                        0xaa, 0xbb, 0xcc};
  ParsedVersionResponse parsed;

  ASSERT_TRUE(parseVersionResponse(payload.data(), payload.size(), parsed));
  EXPECT_EQ(parsed.major, 6);
  EXPECT_EQ(parsed.minor, 1);
  EXPECT_EQ(parsed.status, 0);
  ASSERT_EQ(parsed.trailingSize, 3u);
  ASSERT_NE(parsed.trailingData, nullptr);
  EXPECT_EQ(parsed.trailingData[0], 0xaa);
  EXPECT_EQ(parsed.trailingData[1], 0xbb);
  EXPECT_EQ(parsed.trailingData[2], 0xcc);
}

TEST(VersionResponseParser, RejectsEveryTruncatedPrefix) {
  const std::array<uint8_t, 6> payload{0x00, 0x06, 0x00, 0x01, 0x00, 0x00};
  for (size_t size = 0; size < VERSION_RESPONSE_PREFIX_SIZE; ++size) {
    ParsedVersionResponse parsed;
    EXPECT_FALSE(parseVersionResponse(payload.data(), size, parsed));
  }
}

TEST(VersionResponseParser, AcceptsExactPrefixWithoutTrailingBytes) {
  const std::array<uint8_t, 6> payload{0x00, 0x01, 0x00, 0x07, 0x00, 0x00};
  ParsedVersionResponse parsed;

  ASSERT_TRUE(parseVersionResponse(payload.data(), payload.size(), parsed));
  EXPECT_EQ(parsed.major, 1);
  EXPECT_EQ(parsed.minor, 7);
  EXPECT_EQ(parsed.status, 0);
  EXPECT_EQ(parsed.trailingSize, 0u);
  EXPECT_EQ(parsed.trailingData, nullptr);
}

TEST(VersionResponseParser, PreservesSignedFailureStatus) {
  const std::array<uint8_t, 6> payload{0x00, 0x06, 0x00, 0x00, 0xff, 0xff};
  ParsedVersionResponse parsed;

  ASSERT_TRUE(parseVersionResponse(payload.data(), payload.size(), parsed));
  EXPECT_EQ(parsed.status, -1);
}

} // namespace aasdk::channel::control

// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <cstddef>
#include <cstdint>

namespace aasdk::channel::control {

constexpr size_t VERSION_RESPONSE_PREFIX_SIZE = 6;

struct ParsedVersionResponse {
  uint16_t major = 0;
  uint16_t minor = 0;
  int16_t status = 0;
  const uint8_t *trailingData = nullptr;
  size_t trailingSize = 0;
};

inline uint16_t readBigEndianUint16(const uint8_t *data) {
  return static_cast<uint16_t>(
      (static_cast<uint16_t>(data[0]) << 8) |
      static_cast<uint16_t>(data[1]));
}

inline bool parseVersionResponse(
    const uint8_t *data, size_t size, ParsedVersionResponse &result) {
  if (data == nullptr || size < VERSION_RESPONSE_PREFIX_SIZE) return false;

  result.major = readBigEndianUint16(data);
  result.minor = readBigEndianUint16(data + 2);
  result.status = static_cast<int16_t>(readBigEndianUint16(data + 4));
  result.trailingData = size > VERSION_RESPONSE_PREFIX_SIZE
      ? data + VERSION_RESPONSE_PREFIX_SIZE : nullptr;
  result.trailingSize = size - VERSION_RESPONSE_PREFIX_SIZE;
  return true;
}

} // namespace aasdk::channel::control

#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>

namespace usoralis::example {

struct Rgb8 {
  std::uint8_t red;
  std::uint8_t green;
  std::uint8_t blue;
};

namespace detail {

inline bool WriteU16(std::ofstream& output, std::uint16_t value) {
  const std::array<unsigned char, 2> Bytes{
      static_cast<unsigned char>(value & 0xffU),
      static_cast<unsigned char>((value >> 8U) & 0xffU),
  };
  output.write(reinterpret_cast<const char*>(Bytes.data()),
               static_cast<std::streamsize>(Bytes.size()));
  return static_cast<bool>(output);
}

inline bool WriteU32(std::ofstream& output, std::uint32_t value) {
  const std::array<unsigned char, 4> Bytes{
      static_cast<unsigned char>(value & 0xffU),
      static_cast<unsigned char>((value >> 8U) & 0xffU),
      static_cast<unsigned char>((value >> 16U) & 0xffU),
      static_cast<unsigned char>((value >> 24U) & 0xffU),
  };
  output.write(reinterpret_cast<const char*>(Bytes.data()),
               static_cast<std::streamsize>(Bytes.size()));
  return static_cast<bool>(output);
}

}  // namespace detail

inline bool WriteBmp24(const char* path, const std::vector<Rgb8>& pixels,
                       std::uint32_t width, std::uint32_t height) {
  constexpr std::uint32_t FileHeaderSize = 14U;
  constexpr std::uint32_t DibHeaderSize = 40U;
  constexpr std::uint32_t PixelOffset = FileHeaderSize + DibHeaderSize;

  if (path == nullptr || width == 0U || height == 0U) {
    return false;
  }
  if (width > static_cast<std::uint32_t>(
                  std::numeric_limits<std::int32_t>::max()) ||
      height > static_cast<std::uint32_t>(
                   std::numeric_limits<std::int32_t>::max())) {
    return false;
  }

  const auto ExpectedPixels =
      static_cast<std::uint64_t>(width) * static_cast<std::uint64_t>(height);
  if (ExpectedPixels != pixels.size()) {
    return false;
  }

  const std::uint64_t RowBytes = static_cast<std::uint64_t>(width) * 3U;
  const std::uint64_t RowStride = (RowBytes + 3U) & ~std::uint64_t{3U};
  const std::uint64_t PixelBytes = RowStride * height;
  const std::uint64_t FileSize = PixelOffset + PixelBytes;
  if (FileSize > std::numeric_limits<std::uint32_t>::max()) {
    return false;
  }

  std::ofstream output(path, std::ios::binary);
  if (!output) {
    return false;
  }

  output.put('B');
  output.put('M');
  if (!detail::WriteU32(output, static_cast<std::uint32_t>(FileSize)) ||
      !detail::WriteU16(output, 0U) || !detail::WriteU16(output, 0U) ||
      !detail::WriteU32(output, PixelOffset) ||
      !detail::WriteU32(output, DibHeaderSize) ||
      !detail::WriteU32(output, width) || !detail::WriteU32(output, height) ||
      !detail::WriteU16(output, 1U) || !detail::WriteU16(output, 24U) ||
      !detail::WriteU32(output, 0U) ||
      !detail::WriteU32(output, static_cast<std::uint32_t>(PixelBytes)) ||
      !detail::WriteU32(output, 0U) || !detail::WriteU32(output, 0U) ||
      !detail::WriteU32(output, 0U) || !detail::WriteU32(output, 0U)) {
    return false;
  }

  for (std::uint32_t row = 0; row < height; ++row) {
    const std::uint32_t SourceY = height - row - 1U;
    for (std::uint32_t x = 0; x < width; ++x) {
      const auto& Pixel =
          pixels[static_cast<std::size_t>(SourceY) * width + x];
      const std::array<unsigned char, 3> Bytes{Pixel.blue, Pixel.green,
                                               Pixel.red};
      output.write(reinterpret_cast<const char*>(Bytes.data()),
                   static_cast<std::streamsize>(Bytes.size()));
    }
    for (std::uint64_t padding = RowBytes; padding < RowStride; ++padding) {
      output.put('\0');
    }
  }

  return static_cast<bool>(output);
}

}  // namespace usoralis::example

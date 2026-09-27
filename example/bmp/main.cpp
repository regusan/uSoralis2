#include "BmpWriter.hpp"

#include <usoralis/Shader.hpp>

#include <glm/vec2.hpp>

#include <cstdint>
#include <iostream>
#include <vector>

namespace {

struct Uniform {};

struct FragmentInput {
  glm::vec2 uv;
};

}  // namespace

int main(int argc, char** argv) {
  constexpr std::uint32_t Width = 256U;
  constexpr std::uint32_t Height = 256U;

  const char* outputPath = argc >= 2 ? argv[1] : "output.bmp";
  const Uniform UniformData{};
  const auto ShaderProgram =
      usoralis::MakeShader<Uniform, FragmentInput, usoralis::example::Rgb8>(
          [](const FragmentInput& input, const Uniform&) {
            return usoralis::example::Rgb8{
                static_cast<std::uint8_t>(input.uv.x * 255.0F),
                static_cast<std::uint8_t>(input.uv.y * 255.0F), 128U};
          });

  const std::size_t PixelCount = static_cast<std::size_t>(Width) * Height;
  std::vector<usoralis::example::Rgb8> pixels(PixelCount);
  for (std::uint32_t y = 0; y < Height; ++y) {
    for (std::uint32_t x = 0; x < Width; ++x) {
      const FragmentInput Input{
          glm::vec2{
              static_cast<float>(x) / static_cast<float>(Width - 1U),
              static_cast<float>(y) / static_cast<float>(Height - 1U),
          },
      };
      pixels[static_cast<std::size_t>(y) * Width + x] =
          ShaderProgram(Input, UniformData);
    }
  }

  const auto WriteResult =
      usoralis::example::WriteBmp24(outputPath, pixels, Width, Height);
  if (!WriteResult) {
    std::cerr << "BMPの書き出しに失敗しました: "
              << usoralis::example::BmpErrorMessage(WriteResult.error()) << " ("
              << outputPath << ")\n";
    return 1;
  }

  std::cout << "BMPを書き出しました: " << outputPath << '\n';
  return 0;
}

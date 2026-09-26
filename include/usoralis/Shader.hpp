#pragma once

#include <type_traits>
#include <utility>

namespace usoralis {

template <typename Uniform, typename In, typename Out, typename Function>
class Shader {
 public:
  explicit constexpr Shader(Function function) : function_(std::move(function)) {}

  constexpr Out operator()(const In& input, const Uniform& uniform) const {
    return function_(input, uniform);
  }

 private:
  Function function_;
};

template <typename Uniform, typename In, typename Out, typename Function>
constexpr auto MakeShader(Function&& function) {
  using StoredFunction = std::decay_t<Function>;
  return Shader<Uniform, In, Out, StoredFunction>(
      std::forward<Function>(function));
}

}  // namespace usoralis

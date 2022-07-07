
#pragma once

#include <string>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

using namespace std::string_literals;

namespace rv = ranges::views;

template <typename T>
class tensor {
  std::vector<int32_t> _shape;
  std::vector<T> _data;

 public:
  // Default ctor for rank-1 array from T
  tensor() : _data{}, _shape{{0}} {}

  // Ctor for rank-0 array from T
  tensor(T data) : _data{data}, _shape{{}} {}

  // Ctor for rank-1 array from std::vector
  tensor(std::vector<T> data) : _data{data}, _shape{{1}} {}

  [[nodiscard]] auto shape() const { return _shape; }
  [[nodiscard]] auto rank() const { return _shape.size(); };
  [[nodiscard]] auto data() const { return _data; };
  [[nodiscard]] auto string() const {
    if (rank() == 1) {
      return _data                                                      //
             | rv::transform([](auto e) { return std::to_string(e); })  //
             | rv::join(' ')                                            //
             | ranges::to<std::string>;
    }
    // TODO: use fmt for this
    return "tensor::string() not implemented for rank"s +
           std::to_string(rank());
  }
};
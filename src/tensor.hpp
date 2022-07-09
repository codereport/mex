
#pragma once

#include <span>
#include <string>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

using namespace std::string_literals;

namespace rv = ranges::views;

template <typename T>
auto to_string(T t) {
    if (t.rank() == 0) { return std::to_string(t.data().front()); }
    if (t.rank() == 1) {
        return t.data()                                                   //
               | rv::transform([](auto e) { return std::to_string(e); })  //
               | rv::join(' ')                                            //
               | ranges::to<std::string>;
    }
    // TODO: use fmt for this
    return "tensor::string() not implemented for rank "s + std::to_string(t.rank());
}

template <typename T>
class tensor {
    std::vector<int32_t> _shape;
    std::vector<T> _data;

   public:
    // Default ctor for rank-1 array from T
    tensor() : _data{}, _shape{{0}} {}

    // Ctor for rank-0 array from T
    tensor(T data) : _data{data}, _shape{} {}

    // Ctor for rank-1 array from std::vector
    tensor(std::vector<T> data) : _data{data}, _shape{{static_cast<int32_t>(_data.size())}} {}

    [[nodiscard]] auto shape() const { return _shape; }
    [[nodiscard]] auto rank() const { return _shape.size(); };
    [[nodiscard]] auto data() -> std::span<T> { return _data; };
};

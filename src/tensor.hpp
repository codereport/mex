
#pragma once

#include <span>
#include <string>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

using namespace std::string_literals;

namespace rv = ranges::views;

template <typename T>
class tensor {
    std::vector<int32_t> _shape;
    std::vector<T> _data;

   public:
    tensor() : _data{}, _shape(1, 0) {}                                        // Default ctor for rank-1 array from T
    tensor(T data) : _data{data}, _shape{} {}                                  // Ctor for rank-0 array from T
    tensor(std::vector<T> data) : _data{data}, _shape(1, data.size()) {}       // Ctor for rank-1 array from std::vector
    tensor(std::vector<T> d, std::vector<int32_t> s) : _data{d}, _shape(s) {}  // Ctor for rank-1 array from std::vector

    // clang-format off
    [[nodiscard]] auto shape() const           { return _shape;        }
    [[nodiscard]] auto rank () const           { return _shape.size(); };
    [[nodiscard]] auto data () -> std::span<T> { return _data;         };
    [[nodiscard]] auto data () const           { return _data;         };
    // clang-format on

    bool operator==(tensor<T> const& t) const { return _data == t.data() && _shape == t.shape(); }
};

template <typename T>
auto to_string(tensor<T> t) {
    if (t.rank() == 0) { return std::to_string(t.data().front()); }
    auto join_with_spaces = [](auto x) { return fmt::format("{}", fmt::join(x, " ")); };
    if (t.rank() == 1) { return join_with_spaces(t.data()); }
    if (t.rank() == 2) {
        auto const columns = t.shape().back();
        auto const lines   = t.data() | rv::chunk(columns) | rv::transform(join_with_spaces);
        return fmt::format("{}", fmt::join(lines, "\n"));
    }
    return fmt::format("tensor::string() not implemented for rank {}", t.rank());
}

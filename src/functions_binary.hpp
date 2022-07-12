
#pragma once

#include <numeric>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>

#include <utf8.h>

#include <fmt/core.h>

#include <combinators.hpp>

#include <error.hpp>
#include <token.hpp>

using namespace combinators;

auto is_binary_function(token function) -> bool {
    auto const& val = function.u16string_value();
    return val == utf8::utf8to16("+") or val == utf8::utf8to16("×") or val == utf8::utf8to16("÷") or
           val == utf8::utf8to16("-") or val == utf8::utf8to16("⍴");
}

auto binary_generic_scalar(tensor<int> l, tensor<int> r, auto binop, std::string op) -> expected_tensor {
    if (l.shape() == r.shape()) {
        auto const data = rv::zip_with(binop, l.data(), r.data()) | ranges::to<std::vector>;
        return tensor{data, l.shape()};
    } else if (l.rank() == 0) {
        auto const val  = l.data().front();
        auto const data = r.data() | rv::transform(std::bind_front(binop, val)) | ranges::to<std::vector>;
        return tensor{data, r.shape()};
    } else if (r.rank() == 0) {
        return make_tensor_error(error_type::NOT_IMPLEMENTED_YET, fmt::format("binary {}, right rank == 0", op));
    }

    return make_tensor_error(error_type::DOMAIN,
                             fmt::format("binary {} with left & right unequal shapes not supported", op));
}

// clang-format off
auto binary_minus   (tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _sub_,  "-"); }
auto binary_multiply(tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _mul_,  "×"); }
auto binary_plus    (tensor<int> l, tensor<int> r) -> expected_tensor { return binary_generic_scalar(l, r, _plus_, "+"); }
// clang-format on

auto binary_reshape(tensor<int> l, tensor<int> r) -> expected_tensor {
    auto const n     = std::accumulate(l.data().begin(), l.data().end(), 1, _mul_);
    auto const data  = r.data() | rv::cycle | rv::take(n) | ranges::to<std::vector>;
    auto const shape = l.data() | ranges::to<std::vector>;
    return tensor{data, shape};
}
